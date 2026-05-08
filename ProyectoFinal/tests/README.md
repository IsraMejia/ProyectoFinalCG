# Train Animation Bug Condition Test

## Overview

This test demonstrates the bug where the Train animation system reports `playback=ON` but the Train model remains stationary during playback.

**CRITICAL**: This test is EXPECTED TO FAIL on unfixed code. The failure confirms that the bug exists.

## Test Properties

### Property 1: Bug Condition - Train Animation Updates During Playback

**Validates: Requirements 2.1, 2.2, 2.3**

For any frame where `trenAnimSystem.isPlaying() == true` and `applyTransformationToModel()` is called, the Train's position and rotation SHALL update from their initial values according to interpolated keyframe data.

## Test Cases

### Test Case 1: Train Position Update During Playback

- Load `tren_runtime.kf` (16 keyframes)
- Activate playback mode
- Advance 10 frames
- Assert that `tren.position` has moved from initial position (25.86, -3.28, 65.00)

**Expected on UNFIXED code**: FAIL
- Expected: `tren.position` moves toward keyframe 1 position (36.81, -3.28, -57.74)
- Actual: `tren.position` remains at initial position

### Test Case 2: Train Rotation Update During Playback

- Load `tren_runtime.kf` (16 keyframes)
- Activate playback mode
- Advance 10 frames
- Assert that `tren.rotationY` has changed from initial value (10.0f)

**Expected on UNFIXED code**: FAIL
- Expected: `tren.rotationY` updates according to keyframe data
- Actual: `tren.rotationY` remains at 10.0f

## Running the Test

### Option 1: Using Makefile (MinGW/GCC)

```bash
cd ProyectoFinal/tests
make test
```

### Option 2: Manual Compilation

```bash
cd ProyectoFinal/tests
g++ -std=c++11 -I../include -I../glm -I.. train_animation_bug_condition_test.cpp \
    ../Integrantes/Isra/animacion_KF.cpp \
    ../Integrantes/Isra/tren.cpp \
    ../dependencias/Model.cpp \
    ../dependencias/Mesh.cpp \
    ../dependencias/Material.cpp \
    -L../lib -lglew32 -lglfw3 -lopengl32 -lgdi32 \
    -o train_animation_bug_test.exe

./train_animation_bug_test.exe
```

### Option 3: Using Visual Studio

1. Add `train_animation_bug_condition_test.cpp` to the ProyectoFinal project
2. Build the project
3. Run the test executable

## Expected Output

### On UNFIXED Code (Bug Exists)

```
========================================
Train Animation Bug Condition Test
========================================

This test demonstrates the bug where Train animation
reports playback=ON but the Train model remains stationary.

EXPECTED OUTCOME ON UNFIXED CODE: TEST FAILS
(Failure confirms the bug exists)

=== Test Case 1: Train Position Update During Playback ===
Initial Train position: (25.86, -3.28, 65.00)
Loaded 16 keyframes
Playback activated: ON

Simulating 10 frames of animation...
Frame 0: position = (25.86, -3.28, 65.00)
Frame 4: position = (25.86, -3.28, 65.00)
Frame 9: position = (25.86, -3.28, 65.00)

Final Train position after 10 frames: (25.86, -3.28, 65.00)

--- TEST RESULT ---
Position changed: NO

*** BUG DETECTED ***
Expected: Train position should move toward keyframe 1 position
  Keyframe 0: (51.45, -3.28, -6.70)
  Keyframe 1: (36.81, -3.28, -57.74)
Actual: Train position remains at initial position
  Initial: (25.86, -3.28, 65.00)
  Final:   (25.86, -3.28, 65.00)

Root Cause: applyTransformationToModel() uses dummy variables
  dummyRotX and dummyRotZ intercept rotation updates
  These values are discarded at the end of each frame

========================================
TEST FAILED (EXPECTED ON UNFIXED CODE)
========================================

This failure confirms the bug exists.
The Train animation system does not update the Train's
position and rotation during playback.
```

### On FIXED Code (Bug Resolved)

```
========================================
Train Animation Bug Condition Test
========================================

=== Test Case 1: Train Position Update During Playback ===
Initial Train position: (25.86, -3.28, 65.00)
Loaded 16 keyframes
Playback activated: ON

Simulating 10 frames of animation...
Frame 0: position = (50.00, -3.28, -10.50)
Frame 4: position = (42.30, -3.28, -35.20)
Frame 9: position = (38.15, -3.28, -55.10)

Final Train position after 10 frames: (38.15, -3.28, -55.10)

--- TEST RESULT ---
Position changed: YES

Test Case 1: PASSED (bug is fixed)

=== Test Case 2: Train Rotation Update During Playback ===
Initial Train rotationY: 10.0
Loaded 16 keyframes
Playback activated: ON

Simulating 10 frames of animation...
Frame 0: rotationY = -450.5
Frame 4: rotationY = -480.2
Frame 9: rotationY = -510.8

Final Train rotationY after 10 frames: -510.8

--- TEST RESULT ---
Rotation changed: YES

Test Case 2: PASSED (bug is fixed)

========================================
ALL TESTS PASSED
========================================

The bug has been FIXED!
Train animation now updates position and rotation correctly.
```

## Root Cause

The bug is caused by using dummy variables in `main.cpp` lines 614-616:

```cpp
float dummyRotX = 0.0f;
float dummyRotZ = 0.0f;
trenAnimSystem.applyTransformationToModel(tren.position, dummyRotX, 
    tren.rotationY, dummyRotZ);
```

The `applyTransformationToModel()` function updates all parameters by reference, but the dummy variables intercept the rotation updates and discard them at the end of each frame.

## Fix Implementation

The fix requires:

1. Add `rotationX` and `rotationZ` members to the `Tren` class
2. Initialize these members in the `Tren` constructor
3. Remove the dummy variables from `main.cpp`
4. Pass direct references to `tren.rotationX` and `tren.rotationZ` in the `applyTransformationToModel()` call

See `design.md` for detailed fix implementation steps.

## Notes

- This test uses a scoped property-based testing approach focused on concrete failing cases
- The test simulates the exact bug condition from the main application
- The test is designed to fail on unfixed code and pass after the fix is applied
- This follows the observation-first methodology for bugfix testing
