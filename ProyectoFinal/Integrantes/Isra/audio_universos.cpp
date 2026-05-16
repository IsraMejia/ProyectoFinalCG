/*
 * Sistema de Audio Posicional 2D - Universos de Edgar Allan Poe
 * Implementa atenuación por zonas basada en la posición de la cámara (solo ejes X, Z)
 * Utiliza miniaudio para reproducción de MP3
 * 
 * Autor: Isra
 * Fecha: Mayo 2026
 */

#include "../../dependencias/miniaudio.h"
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <stdio.h>

// Función auxiliar para clamp (por si glm::clamp no está disponible)
template<typename T>
inline T clamp(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

// ============================================================
// ESTRUCTURAS DE DATOS
// ============================================================

struct AudioTrack {
    ma_decoder decoder;
    ma_device device;
    bool isPlaying;
    float targetVolume;
    float currentVolume;
    
    AudioTrack() : isPlaying(false), targetVolume(0.0f), currentVolume(0.0f) {}
};

struct UniverseZone {
    glm::vec2 supIzq;      // Superior Izquierda
    glm::vec2 supDer;      // Superior Derecha
    glm::vec2 infDer;      // Inferior Derecha
    glm::vec2 infIzq;      // Inferior Izquierda
    glm::vec2 centro;      // Centro del escenario (máxima intensidad)
    
    AudioTrack* audioTrack; // Puntero a la pista de audio
    
    UniverseZone(glm::vec3 si, glm::vec3 sd, glm::vec3 id, glm::vec3 ii, glm::vec3 c)
        : supIzq(si.x, si.z), supDer(sd.x, sd.z), 
          infDer(id.x, id.z), infIzq(ii.x, ii.z),
          centro(c.x, c.z), audioTrack(nullptr) {}
};

// ============================================================
// CONSTANTES DE CONFIGURACIÓN
// ============================================================

const float VOLUMEN_FONDO = 0.2f;        // 20% volumen constante para música de Poe
const float VOLUMEN_MAX_UNIVERSO = 0.9f; // 90% en el centro del universo
const float VOLUMEN_MIN_UNIVERSO = 0.3f; // 30% en los bordes del universo
const float FADE_DISTANCE = 5.0f;        // Distancia de fade suave al salir de la zona
const float VOLUME_SMOOTH_SPEED = 0.05f; // Velocidad de suavizado de volumen

// ============================================================
// VARIABLES GLOBALES DE AUDIO
// ============================================================

AudioTrack trackPoe;      // Música de fondo (Poe)
AudioTrack trackAndrea;   // Música de Andrea (FNaF)
AudioTrack trackCeci;     // Música de Ceci (Miku)
AudioTrack trackIsra;     // Música de Isra (Halo)

// ============================================================
// DEFINICIÓN DE ZONAS
// ============================================================

// Zona Andrea (FNaF)
UniverseZone zoneAndrea(
    glm::vec3(-139.98f, 33.55f, -110.76f),  // SupIzq
    glm::vec3(-20.29f, 27.73f, -133.96f),   // SupDer
    glm::vec3(1.15f, 15.75f, -4.77f),       // InfDer
    glm::vec3(-62.98f, 14.92f, 50.85f),     // InfIzq
    glm::vec3(-45.64f, 11.37f, -65.26f)     // Centro
);

// Zona Ceci (Miku)
UniverseZone zoneCeci(
    glm::vec3(-3.54f, 20.44f, -119.76f),    // SupIzq
    glm::vec3(110.62f, 12.20f, -77.76f),    // SupDer
    glm::vec3(125.01f, 9.58f, 49.80f),      // InfDer
    glm::vec3(9.89f, 5.60f, 47.61f),        // InfIzq
    glm::vec3(72.97f, 3.78f, -3.17f)        // Centro
);

// Zona Isra (Halo)
UniverseZone zoneIsra(
    glm::vec3(-11.81f, 3.92f, 82.06f),      // SupIzq
    glm::vec3(72.80f, 2.54f, 64.03f),       // SupDer
    glm::vec3(91.01f, 3.97f, 119.99f),      // InfDer
    glm::vec3(2.17f, 0.70f, 118.05f),       // InfIzq
    glm::vec3(39.58f, 4.70f, 106.69f)       // Centro
);

// ============================================================
// CALLBACK DE AUDIO (miniaudio)
// ============================================================

void audio_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    AudioTrack* pTrack = (AudioTrack*)pDevice->pUserData;
    if (pTrack == nullptr) return;
    
    // Leer frames del decoder
    ma_uint64 framesRead = 0;
    ma_decoder_read_pcm_frames(&pTrack->decoder, pOutput, frameCount, &framesRead);
    
    // Aplicar volumen con suavizado
    float* pOutputF32 = (float*)pOutput;
    for (ma_uint32 i = 0; i < frameCount * pDevice->playback.channels; ++i) {
        // Suavizar transición de volumen
        pTrack->currentVolume += (pTrack->targetVolume - pTrack->currentVolume) * VOLUME_SMOOTH_SPEED;
        pOutputF32[i] *= pTrack->currentVolume;
    }
    
    // Si llegamos al final, reiniciar (loop)
    if (framesRead < (ma_uint64)frameCount) {
        ma_decoder_seek_to_pcm_frame(&pTrack->decoder, 0);
    }
}

// ============================================================
// FUNCIONES AUXILIARES DE GEOMETRÍA
// ============================================================

float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool isPointInTriangle(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3) {
    float d1 = sign(pt, v1, v2);
    float d2 = sign(pt, v2, v3);
    float d3 = sign(pt, v3, v1);
    
    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    
    return !(hasNeg && hasPos);
}

bool isPointInQuad(glm::vec2 point, const UniverseZone& zone) {
    bool inTriangle1 = isPointInTriangle(point, zone.supIzq, zone.supDer, zone.infDer);
    bool inTriangle2 = isPointInTriangle(point, zone.supIzq, zone.infDer, zone.infIzq);
    return inTriangle1 || inTriangle2;
}

float distanceToSegment(glm::vec2 point, glm::vec2 segStart, glm::vec2 segEnd) {
    glm::vec2 segment = segEnd - segStart;
    glm::vec2 pointVec = point - segStart;
    
    float segLengthSq = glm::dot(segment, segment);
    if (segLengthSq == 0.0f) return glm::length(pointVec);
    
    float t = clamp(glm::dot(pointVec, segment) / segLengthSq, 0.0f, 1.0f);
    glm::vec2 projection = segStart + t * segment;
    
    return glm::length(point - projection);
}

float distanceToBorder(glm::vec2 point, const UniverseZone& zone) {
    float d1 = distanceToSegment(point, zone.supIzq, zone.supDer);
    float d2 = distanceToSegment(point, zone.supDer, zone.infDer);
    float d3 = distanceToSegment(point, zone.infDer, zone.infIzq);
    float d4 = distanceToSegment(point, zone.infIzq, zone.supIzq);
    return std::min({d1, d2, d3, d4});
}

float calculateUniverseVolume(glm::vec2 cameraPos2D, const UniverseZone& zone) {
    bool isInside = isPointInQuad(cameraPos2D, zone);
    
    if (!isInside) {
        float distToBorder = distanceToBorder(cameraPos2D, zone);
        if (distToBorder > FADE_DISTANCE) return 0.0f;
        float fadeRatio = 1.0f - (distToBorder / FADE_DISTANCE);
        return VOLUMEN_MIN_UNIVERSO * fadeRatio;
    }
    
    float distToCenter = glm::length(cameraPos2D - zone.centro);
    float maxRadius = distanceToBorder(zone.centro, zone);
    float normalizedDist = clamp(distToCenter / maxRadius, 0.0f, 1.0f);
    
    return glm::mix(VOLUMEN_MAX_UNIVERSO, VOLUMEN_MIN_UNIVERSO, normalizedDist);
}

// ============================================================
// FUNCIONES DE INICIALIZACIÓN Y GESTIÓN DE AUDIO
// ============================================================

bool initAudioTrack(AudioTrack* track, const char* filepath) {
    // Inicializar decoder
    ma_result result = ma_decoder_init_file(filepath, NULL, &track->decoder);
    if (result != MA_SUCCESS) {
        printf("Error: No se pudo cargar el archivo de audio: %s\n", filepath);
        return false;
    }
    
    // Configurar dispositivo de audio
    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = track->decoder.outputFormat;
    deviceConfig.playback.channels = track->decoder.outputChannels;
    deviceConfig.sampleRate        = track->decoder.outputSampleRate;
    deviceConfig.dataCallback      = audio_callback;
    deviceConfig.pUserData         = track;
    
    result = ma_device_init(NULL, &deviceConfig, &track->device);
    if (result != MA_SUCCESS) {
        printf("Error: No se pudo inicializar el dispositivo de audio\n");
        ma_decoder_uninit(&track->decoder);
        return false;
    }
    
    track->isPlaying = false;
    track->targetVolume = 0.0f;
    track->currentVolume = 0.0f;
    
    return true;
}

void startAudioTrack(AudioTrack* track) {
    if (!track->isPlaying) {
        ma_device_start(&track->device);
        track->isPlaying = true;
    }
}

void stopAudioTrack(AudioTrack* track) {
    if (track->isPlaying) {
        ma_device_stop(&track->device);
        track->isPlaying = false;
    }
}

void setAudioVolume(AudioTrack* track, float volume) {
    track->targetVolume = clamp(volume, 0.0f, 1.0f);
}

void cleanupAudioTrack(AudioTrack* track) {
    if (track->isPlaying) {
        ma_device_stop(&track->device);
    }
    ma_device_uninit(&track->device);
    ma_decoder_uninit(&track->decoder);
}

// ============================================================
// FUNCIÓN PRINCIPAL DE INICIALIZACIÓN
// ============================================================

bool initAudioSystem() {
    printf("Inicializando sistema de audio...\n");
    
    // Cargar música de fondo (Poe)
    if (!initAudioTrack(&trackPoe, "audio/poe.mp3")) {
        printf("Advertencia: No se pudo cargar audio/poe.mp3\n");
    } else {
        setAudioVolume(&trackPoe, VOLUMEN_FONDO);
        startAudioTrack(&trackPoe);
        printf("✓ Cargado: audio/poe.mp3\n");
    }
    
    // Cargar música de Andrea (FNaF)
    if (!initAudioTrack(&trackAndrea, "audio/five_nights_at _freddys.mp3")) {
        printf("Advertencia: No se pudo cargar audio/five_nights_at _freddys.mp3\n");
    } else {
        zoneAndrea.audioTrack = &trackAndrea;
        startAudioTrack(&trackAndrea);
        printf("✓ Cargado: audio/five_nights_at _freddys.mp3\n");
    }
    
    // Cargar música de Ceci (Miku)
    if (!initAudioTrack(&trackCeci, "audio/miku.mp3")) {
        printf("Advertencia: No se pudo cargar audio/miku.mp3\n");
    } else {
        zoneCeci.audioTrack = &trackCeci;
        startAudioTrack(&trackCeci);
        printf("✓ Cargado: audio/miku.mp3\n");
    }
    
    // Cargar música de Isra (Halo)
    if (!initAudioTrack(&trackIsra, "audio/halo.mp3")) {
        printf("Advertencia: No se pudo cargar audio/halo.mp3\n");
    } else {
        zoneIsra.audioTrack = &trackIsra;
        startAudioTrack(&trackIsra);
        printf("✓ Cargado: audio/halo.mp3\n");
    }
    
    printf("Sistema de audio inicializado correctamente.\n");
    return true;
}

// ============================================================
// FUNCIÓN PRINCIPAL DE ACTUALIZACIÓN
// ============================================================

void updateAudio(glm::vec3 cameraPos) {
    // Convertir posición 3D a 2D (ignorar altura Y)
    glm::vec2 cameraPos2D(cameraPos.x, cameraPos.z);
    
    // Música de fondo (Poe) - siempre al 20%
    setAudioVolume(&trackPoe, VOLUMEN_FONDO);
    
    // Música de Andrea (FNaF)
    float volumenAndrea = calculateUniverseVolume(cameraPos2D, zoneAndrea);
    setAudioVolume(&trackAndrea, volumenAndrea);
    
    // Música de Ceci (Miku)
    float volumenCeci = calculateUniverseVolume(cameraPos2D, zoneCeci);
    setAudioVolume(&trackCeci, volumenCeci);
    
    // Música de Isra (Halo)
    float volumenIsra = calculateUniverseVolume(cameraPos2D, zoneIsra);
    setAudioVolume(&trackIsra, volumenIsra);
    
    // Debug opcional (descomentar para ver valores)
    // printf("Volúmenes - Andrea: %.2f | Ceci: %.2f | Isra: %.2f\n", 
    //        volumenAndrea, volumenCeci, volumenIsra);
}

// ============================================================
// FUNCIÓN DE LIMPIEZA
// ============================================================

void cleanupAudioSystem() {
    printf("Cerrando sistema de audio...\n");
    cleanupAudioTrack(&trackPoe);
    cleanupAudioTrack(&trackAndrea);
    cleanupAudioTrack(&trackCeci);
    cleanupAudioTrack(&trackIsra);
    printf("Sistema de audio cerrado.\n");
}
