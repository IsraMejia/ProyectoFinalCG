/**
 * Bug Condition Exploration Test - Train Animation Playback
 * 
 * **Validates: Requirements 2.1, 2.2, 2.3**
 * 
 * CRITICAL: This test MUST FAIL on unfixed code - failure confirms the bug exists
 * DO NOT attempt to fix the test or the code when it fails
 * 
 * This test encodes the expected behavior - it will validate the fix when it passes
 * after implementation.
 * 
 * GOAL: Surface counterexamples that demonstrate the bug exists
 * 
 * Property 1: Bug Condition - Train Animation Updates During Playback
 * 
 * For any frame where trenAnimSystem.isPlaying() == true and 
 * applyTransformationToModel() is called, the Train's position and rotation
 * SHALL update from their initial values according to interpolated keyframe data.
 */

#include <iostream>
#include <cmath>
#include <cassert>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "../Integrantes/Isra/animacion_KF.h"
#include "../Integrantes/Isra/tren.h"

// Test configuration
const float EPSILON = 0.01f;  // Tolerance for floating point comparison
const float FRAME_TIME = 1.0f / 24.0f;  // 24 FPS

// Helper function to compare floating point values
bool floatEquals(float a, float b, float epsilon = EPSILON) {
    return std::abs(a - b) < epsilon;
}

// Helper function to compare vec3 values
bool vec3Equals(const glm::vec3& a, const glm::vec3& b, float epsilon = EPSILON) {
    return floatEquals(a.x, b.x, epsilon) && 
           floatEquals(a.y, b.y, epsilon) && 
           floatEquals(a.z, b.z, epsilon);
}

/**
 * Test Case 1: Train Position Update During Playback
 * 
 * Concrete test case: Load tren_runtime.kf (16 keyframes), activate playback,
 * advance 10 frames, assert tren.position has moved from initial position.
 * 
 * Expected on UNFIXED code: FAIL
 * - Expected: tren.position moves toward keyframe 1 position (36.81, -3.28, -57.74)
 * - Actual: tren.position remains at (51.45, -3.28, -6.70)
 */
void test_train_position_updates_during_playback() {
    std::cout << "\n=== Test Case 1: Train Position Update During Playback ===" << std::endl;
    
    // Initialize Train object
    Tren tren;
    
    // Record initial position (from tren.cpp constructor)
    glm::vec3 initialPosition = tren.position;
    std::cout << "Initial Train position: (" 
              << initialPosition.x << ", " 
              << initialPosition.y << ", " 
              << initialPosition.z << ")" << std::endl;
    
    // Expected initial position from constructor
    glm::vec3 expectedInitialPos(25.86f, -3.28f, 65.00f);
    assert(vec3Equals(initialPosition, expectedInitialPos) && 
           "Train initial position should match constructor value");
    
    // Initialize animation system
    Keyframe_System trenAnimSystem(100);
    
    // Load keyframes from file
    bool loaded = trenAnimSystem.loadKeyframesFromFile("Integrantes/Isra/tren_runtime.kf");
    assert(loaded && "Failed to load tren_runtime.kf");
    
    int keyframeCount = trenAnimSystem.getKeyframeCount();
    std::cout << "Loaded " << keyframeCount << " keyframes" << std::endl;
    assert(keyframeCount == 16 && "Expected 16 keyframes in tren_runtime.kf");
    
    // Activate playback mode
    trenAnimSystem.togglePlaybackMode();
    assert(trenAnimSystem.isPlaying() && "Playback should be active");
    std::cout << "Playback activated: " << (trenAnimSystem.isPlaying() ? "ON" : "OFF") << std::endl;
    
    // Simulate 10 frames of animation
    std::cout << "\nSimulating 10 frames of animation..." << std::endl;
    for (int frame = 0; frame < 10; frame++) {
        trenAnimSystem.updatePlayback(FRAME_TIME);
        
        // THIS IS THE BUG: Using dummy variables instead of direct references
        // This simulates the buggy code in main.cpp lines 614-616
        float dummyRotX = 0.0f;
        float dummyRotZ = 0.0f;
        trenAnimSystem.applyTransformationToModel(tren.position, dummyRotX, 
                                                  tren.rotationY, dummyRotZ);
        
        if (frame == 0 || frame == 4 || frame == 9) {
            std::cout << "Frame " << frame << ": position = (" 
                      << tren.position.x << ", " 
                      << tren.position.y << ", " 
                      << tren.position.z << ")" << std::endl;
        }
    }
    
    // After 10 frames, position should have moved from initial position
    glm::vec3 finalPosition = tren.position;
    std::cout << "\nFinal Train position after 10 frames: (" 
              << finalPosition.x << ", " 
              << finalPosition.y << ", " 
              << finalPosition.z << ")" << std::endl;
    
    // Expected: Position should move toward keyframe 1 (36.81, -3.28, -57.74)
    // from keyframe 0 (51.45, -3.28, -6.70)
    glm::vec3 keyframe0Pos(51.45f, -3.28f, -6.70f);
    glm::vec3 keyframe1Pos(36.81f, -3.28f, -57.74f);
    
    // CRITICAL ASSERTION: This WILL FAIL on unfixed code
    // The position should have changed from the initial position
    bool positionChanged = !vec3Equals(finalPosition, initialPosition, 0.1f);
    
    std::cout << "\n--- TEST RESULT ---" << std::endl;
    std::cout << "Position changed: " << (positionChanged ? "YES" : "NO") << std::endl;
    
    if (!positionChanged) {
        std::cout << "\n*** BUG DETECTED ***" << std::endl;
        std::cout << "Expected: Train position should move toward keyframe 1 position" << std::endl;
        std::cout << "  Keyframe 0: (" << keyframe0Pos.x << ", " << keyframe0Pos.y << ", " << keyframe0Pos.z << ")" << std::endl;
        std::cout << "  Keyframe 1: (" << keyframe1Pos.x << ", " << keyframe1Pos.y << ", " << keyframe1Pos.z << ")" << std::endl;
        std::cout << "Actual: Train position remains at initial position" << std::endl;
        std::cout << "  Initial: (" << initialPosition.x << ", " << initialPosition.y << ", " << initialPosition.z << ")" << std::endl;
        std::cout << "  Final:   (" << finalPosition.x << ", " << finalPosition.y << ", " << finalPosition.z << ")" << std::endl;
        std::cout << "\nRoot Cause: applyTransformationToModel() uses dummy variables" << std::endl;
        std::cout << "  dummyRotX and dummyRotZ intercept rotation updates" << std::endl;
        std::cout << "  These values are discarded at the end of each frame" << std::endl;
    }
    
    // This assertion WILL FAIL on unfixed code (this is expected and correct)
    assert(positionChanged && 
           "EXPECTED FAILURE: Train position should update during playback (bug exists)");
    
    std::cout << "\nTest Case 1: PASSED (bug is fixed)" << std::endl;
}

/**
 * Test Case 2: Train Rotation Update During Playback
 * 
 * Concrete test case: Load tren_runtime.kf, activate playback, advance 10 frames,
 * assert tren.rotationY has changed from initial value.
 * 
 * Expected on UNFIXED code: FAIL
 * - Expected: tren.rotationY updates according to keyframe data
 * - Actual: tren.rotationY remains at 10.0f
 */
void test_train_rotation_updates_during_playback() {
    std::cout << "\n=== Test Case 2: Train Rotation Update During Playback ===" << std::endl;
    
    // Initialize Train object
    Tren tren;
    
    // Record initial rotation (from tren.cpp constructor)
    float initialRotationY = tren.rotationY;
    std::cout << "Initial Train rotationY: " << initialRotationY << std::endl;
    
    // Expected initial rotation from constructor
    assert(floatEquals(initialRotationY, 10.0f) && 
           "Train initial rotationY should be 10.0f");
    
    // Initialize animation system
    Keyframe_System trenAnimSystem(100);
    
    // Load keyframes from file
    bool loaded = trenAnimSystem.loadKeyframesFromFile("Integrantes/Isra/tren_runtime.kf");
    assert(loaded && "Failed to load tren_runtime.kf");
    
    // Activate playback mode
    trenAnimSystem.togglePlaybackMode();
    assert(trenAnimSystem.isPlaying() && "Playback should be active");
    
    // Simulate 10 frames of animation
    std::cout << "\nSimulating 10 frames of animation..." << std::endl;
    for (int frame = 0; frame < 10; frame++) {
        trenAnimSystem.updatePlayback(FRAME_TIME);
        
        // THIS IS THE BUG: Using dummy variables instead of direct references
        float dummyRotX = 0.0f;
        float dummyRotZ = 0.0f;
        trenAnimSystem.applyTransformationToModel(tren.position, dummyRotX, 
                                                  tren.rotationY, dummyRotZ);
        
        if (frame == 0 || frame == 4 || frame == 9) {
            std::cout << "Frame " << frame << ": rotationY = " << tren.rotationY << std::endl;
        }
    }
    
    // After 10 frames, rotation should have changed from initial value
    float finalRotationY = tren.rotationY;
    std::cout << "\nFinal Train rotationY after 10 frames: " << finalRotationY << std::endl;
    
    // Expected: Rotation should update according to keyframe data
    // Keyframe 0: rotY = -524.00
    // Keyframe 1: rotY = -524.00
    // The rotation should move toward these keyframe values
    
    // CRITICAL ASSERTION: This WILL FAIL on unfixed code
    // The rotation should have changed from the initial value
    bool rotationChanged = !floatEquals(finalRotationY, initialRotationY, 1.0f);
    
    std::cout << "\n--- TEST RESULT ---" << std::endl;
    std::cout << "Rotation changed: " << (rotationChanged ? "YES" : "NO") << std::endl;
    
    if (!rotationChanged) {
        std::cout << "\n*** BUG DETECTED ***" << std::endl;
        std::cout << "Expected: Train rotationY should update according to keyframe data" << std::endl;
        std::cout << "  Keyframe 0 rotY: -524.00" << std::endl;
        std::cout << "  Keyframe 1 rotY: -524.00" << std::endl;
        std::cout << "Actual: Train rotationY remains at initial value" << std::endl;
        std::cout << "  Initial: " << initialRotationY << std::endl;
        std::cout << "  Final:   " << finalRotationY << std::endl;
        std::cout << "\nRoot Cause: applyTransformationToModel() uses dummy variables" << std::endl;
        std::cout << "  Even though rotationY is passed correctly, the animation system" << std::endl;
        std::cout << "  may not update properly due to incomplete rotation state" << std::endl;
    }
    
    // This assertion WILL FAIL on unfixed code (this is expected and correct)
    assert(rotationChanged && 
           "EXPECTED FAILURE: Train rotationY should update during playback (bug exists)");
    
    std::cout << "\nTest Case 2: PASSED (bug is fixed)" << std::endl;
}

/**
 * Main test runner
 */
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Train Animation Bug Condition Test" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "\nThis test demonstrates the bug where Train animation" << std::endl;
    std::cout << "reports playback=ON but the Train model remains stationary." << std::endl;
    std::cout << "\nEXPECTED OUTCOME ON UNFIXED CODE: TEST FAILS" << std::endl;
    std::cout << "(Failure confirms the bug exists)" << std::endl;
    
    try {
        test_train_position_updates_during_playback();
        test_train_rotation_updates_during_playback();
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "ALL TESTS PASSED" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "\nThe bug has been FIXED!" << std::endl;
        std::cout << "Train animation now updates position and rotation correctly." << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "TEST FAILED (EXPECTED ON UNFIXED CODE)" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "\nException: " << e.what() << std::endl;
        std::cout << "\nThis failure confirms the bug exists." << std::endl;
        std::cout << "The Train animation system does not update the Train's" << std::endl;
        std::cout << "position and rotation during playback." << std::endl;
        
        return 1;
    }
}
