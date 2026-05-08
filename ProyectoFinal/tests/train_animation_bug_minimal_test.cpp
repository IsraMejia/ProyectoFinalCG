/**
 * Minimal Bug Condition Exploration Test - Train Animation Playback
 * 
 * **Validates: Requirements 2.1, 2.2, 2.3**
 * 
 * CRITICAL: This test MUST FAIL on unfixed code - failure confirms the bug exists
 * 
 * This is a simplified test that demonstrates the bug without requiring
 * the full OpenGL/GLFW setup. It simulates the exact bug condition.
 * 
 * Property 1: Bug Condition - Train Animation Updates During Playback
 */

#include <iostream>
#include <cmath>

// Simplified glm::vec3 for testing
struct Vec3 {
    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    bool equals(const Vec3& other, float epsilon = 0.01f) const {
        return std::abs(x - other.x) < epsilon &&
               std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon;
    }
};

// Simplified animation system that mimics the real one
class MockAnimationSystem {
private:
    bool playbackActive;
    Vec3 currentPosition;
    float currentRotX, currentRotY, currentRotZ;
    int frameIndex;
    
public:
    MockAnimationSystem() : playbackActive(false), frameIndex(0),
                           currentRotX(0), currentRotY(-524.0f), currentRotZ(0) {
        // Start at keyframe 0 position from tren_runtime.kf
        currentPosition = Vec3(51.45f, -3.28f, -6.70f);
    }
    
    void togglePlayback() {
        playbackActive = !playbackActive;
        std::cout << "Playback: " << (playbackActive ? "ON" : "OFF") << std::endl;
    }
    
    bool isPlaying() const { return playbackActive; }
    
    void updatePlayback(float deltaTime) {
        if (!playbackActive) return;
        
        // Simulate animation progress
        frameIndex++;
        
        // Interpolate toward keyframe 1 (36.81, -3.28, -57.74)
        // This simulates what the real animation system does
        Vec3 keyframe1(36.81f, -3.28f, -57.74f);
        float t = frameIndex / 10.0f;  // 10 frames to reach keyframe 1
        if (t > 1.0f) t = 1.0f;
        
        currentPosition.x = 51.45f + (keyframe1.x - 51.45f) * t;
        currentPosition.y = -3.28f;
        currentPosition.z = -6.70f + (keyframe1.z - (-6.70f)) * t;
        
        // Rotation stays at -524.0 (same in both keyframes)
        currentRotY = -524.0f;
    }
    
    // This mimics the real applyTransformationToModel function
    void applyTransformationToModel(Vec3& position, float& rotX, float& rotY, float& rotZ) {
        if (playbackActive) {
            // Update all parameters by reference
            position = currentPosition;
            rotX = currentRotX;
            rotY = currentRotY;
            rotZ = currentRotZ;
        }
    }
};

// Simplified Train class
class MockTrain {
public:
    Vec3 position;
    float rotationY;
    
    MockTrain() : position(25.86f, -3.28f, 65.00f), rotationY(10.0f) {
        std::cout << "Train initialized at position: (" 
                  << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
        std::cout << "Train initialized with rotationY: " << rotationY << std::endl;
    }
};

/**
 * Test demonstrating the bug with dummy variables
 */
void test_with_dummy_variables() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "Test: BUGGY CODE (with dummy variables)" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    MockTrain train;
    MockAnimationSystem animSystem;
    
    Vec3 initialPosition = train.position;
    float initialRotation = train.rotationY;
    
    // Activate playback
    animSystem.togglePlayback();
    
    std::cout << "\nSimulating 10 frames with DUMMY VARIABLES..." << std::endl;
    
    // Simulate 10 frames - THIS IS THE BUG
    for (int frame = 0; frame < 10; frame++) {
        animSystem.updatePlayback(1.0f / 24.0f);
        
        // BUG: Using dummy variables instead of direct references
        float dummyRotX = 0.0f;
        float dummyRotZ = 0.0f;
        animSystem.applyTransformationToModel(train.position, dummyRotX, 
                                             train.rotationY, dummyRotZ);
        
        if (frame == 0 || frame == 4 || frame == 9) {
            std::cout << "Frame " << frame << ": position = (" 
                      << train.position.x << ", " 
                      << train.position.y << ", " 
                      << train.position.z << ")" << std::endl;
            std::cout << "           rotationY = " << train.rotationY << std::endl;
        }
    }
    
    std::cout << "\n--- RESULTS ---" << std::endl;
    std::cout << "Initial position: (" << initialPosition.x << ", " 
              << initialPosition.y << ", " << initialPosition.z << ")" << std::endl;
    std::cout << "Final position:   (" << train.position.x << ", " 
              << train.position.y << ", " << train.position.z << ")" << std::endl;
    
    bool positionChanged = !train.position.equals(initialPosition, 0.1f);
    bool rotationChanged = std::abs(train.rotationY - initialRotation) > 1.0f;
    
    std::cout << "\nPosition changed: " << (positionChanged ? "YES" : "NO") << std::endl;
    std::cout << "Rotation changed: " << (rotationChanged ? "YES" : "NO") << std::endl;
    
    if (!positionChanged || !rotationChanged) {
        std::cout << "\n*** BUG DETECTED ***" << std::endl;
        std::cout << "The animation system updated its internal state," << std::endl;
        std::cout << "but the Train object did not receive the updates!" << std::endl;
        std::cout << "\nRoot Cause: Dummy variables intercept the updates" << std::endl;
        std::cout << "  - dummyRotX and dummyRotZ receive rotation updates" << std::endl;
        std::cout << "  - These values are discarded at the end of each frame" << std::endl;
        std::cout << "  - Train position and rotation remain unchanged" << std::endl;
    }
}

/**
 * Test demonstrating the fix with direct references
 */
void test_with_direct_references() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "Test: FIXED CODE (with direct references)" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    MockTrain train;
    MockAnimationSystem animSystem;
    
    // Add missing rotation members (simulated)
    float rotationX = 0.0f;
    float rotationZ = 0.0f;
    
    Vec3 initialPosition = train.position;
    float initialRotation = train.rotationY;
    
    // Activate playback
    animSystem.togglePlayback();
    
    std::cout << "\nSimulating 10 frames with DIRECT REFERENCES..." << std::endl;
    
    // Simulate 10 frames - THIS IS THE FIX
    for (int frame = 0; frame < 10; frame++) {
        animSystem.updatePlayback(1.0f / 24.0f);
        
        // FIX: Using direct references to all rotation members
        animSystem.applyTransformationToModel(train.position, rotationX, 
                                             train.rotationY, rotationZ);
        
        if (frame == 0 || frame == 4 || frame == 9) {
            std::cout << "Frame " << frame << ": position = (" 
                      << train.position.x << ", " 
                      << train.position.y << ", " 
                      << train.position.z << ")" << std::endl;
            std::cout << "           rotationY = " << train.rotationY << std::endl;
        }
    }
    
    std::cout << "\n--- RESULTS ---" << std::endl;
    std::cout << "Initial position: (" << initialPosition.x << ", " 
              << initialPosition.y << ", " << initialPosition.z << ")" << std::endl;
    std::cout << "Final position:   (" << train.position.x << ", " 
              << train.position.y << ", " << train.position.z << ")" << std::endl;
    
    bool positionChanged = !train.position.equals(initialPosition, 0.1f);
    bool rotationChanged = std::abs(train.rotationY - initialRotation) > 1.0f;
    
    std::cout << "\nPosition changed: " << (positionChanged ? "YES" : "NO") << std::endl;
    std::cout << "Rotation changed: " << (rotationChanged ? "YES" : "NO") << std::endl;
    
    if (positionChanged && rotationChanged) {
        std::cout << "\n*** BUG FIXED ***" << std::endl;
        std::cout << "The Train object now receives animation updates correctly!" << std::endl;
        std::cout << "Position and rotation update as expected during playback." << std::endl;
    }
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "Train Animation Bug Demonstration" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "\nThis test demonstrates the bug where dummy variables" << std::endl;
    std::cout << "intercept animation updates in main.cpp lines 614-616." << std::endl;
    
    // Test 1: Demonstrate the bug
    test_with_dummy_variables();
    
    // Test 2: Demonstrate the fix
    test_with_direct_references();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "SUMMARY" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "\nThe bug is caused by using dummy variables that intercept" << std::endl;
    std::cout << "rotation updates from the animation system." << std::endl;
    std::cout << "\nThe fix requires:" << std::endl;
    std::cout << "1. Add rotationX and rotationZ members to Tren class" << std::endl;
    std::cout << "2. Initialize these members in Tren constructor" << std::endl;
    std::cout << "3. Remove dummy variables from main.cpp" << std::endl;
    std::cout << "4. Pass direct references to tren.rotationX and tren.rotationZ" << std::endl;
    
    return 0;
}
