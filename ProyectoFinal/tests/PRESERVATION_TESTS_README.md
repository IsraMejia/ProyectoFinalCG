# Preservation Property Tests - Train Animation Playback Fix

## Overview

These tests verify that existing functionality is preserved after implementing the bugfix for the Train animation playback issue. The tests follow the **observation-first methodology** for bugfix testing.

**IMPORTANT**: These tests are EXPECTED TO PASS on unfixed code. Passing confirms the baseline behavior that must be preserved.

## Test Properties

### Property 2: Preservation - Non-Train Animation Behavior

**Validates: Requirements 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7**

For any animation system call that is NOT for the Train (Pelican animation, recording mode, keyframe loading, animation reset), the fixed code SHALL produce exactly the same behavior as the original code, preserving all existing functionality.

## Test Cases

### Test 1: Pelican Animation Preservation

**Validates: Requirements 3.1, 3.2**

**Property**: For all frames where `pelicanAnimSystem.isPlaying() == true`, Pelican position and rotation SHALL update according to keyframe data (preserves Pelican animation)

**Test Approach**:
1. Load 24 keyframes for Pelican (simulating `pelican_halo_runtime.kf`)
2. Activate playback mode
3. Advance 10 frames
4. Verify that Pelican position and all three rotation axes (rotX, rotY, rotZ) have changed

**Expected on UNFIXED code**: PASS
- Pelican uses direct references: `pelicanAnimSystem.applyTransformationToModel(haloPelican.position, haloPelican.rotationX, haloPelican.rotationY, haloPelican.rotationZ)`
- All rotation axes update correctly
- Position updates correctly

**Expected on FIXED code**: PASS (behavior preserved)

### Test 2: Train Manual Positioning Preservation

**Validates: Requirement 3.6**

**Property**: For all frames where `trenAnimSystem.isPlaying() == false`, Train SHALL render at its constructor-defined position (preserves manual positioning)

**Test Approach**:
1. Create Train with constructor-defined position (51.45, -3.28, -6.70) and rotationY=10.0f
2. Verify animation is NOT active
3. Simulate 10 frames without animation
4. Verify that Train position and rotation remain unchanged

**Expected on UNFIXED code**: PASS
- When animation is not active, `applyTransformationToModel()` does not modify the Train's state
- Train remains at constructor-defined position
- This behavior works correctly even with dummy variables

**Expected on FIXED code**: PASS (behavior preserved)

### Test 3: Recording Mode Preservation

**Validates: Requirement 3.3**

**Property**: For all recording mode activations, keyframe capture SHALL work correctly for both models (preserves recording functionality)

**Test Approach**:
1. Activate recording mode for Pelican
2. Capture 3 keyframes with different positions and rotations
3. Verify keyframe count is 3
4. Deactivate recording mode
5. Repeat for Train
6. Verify both models can capture keyframes correctly

**Expected on UNFIXED code**: PASS
- Recording mode is independent of the dummy variable issue
- Keyframe capture works correctly for both models
- The bug only affects playback, not recording

**Expected on FIXED code**: PASS (behavior preserved)

### Test 4: Animation Reset Preservation

**Validates: Requirement 3.5**

**Property**: For all animation reset commands, both models SHALL return to keyframe 0 positions (preserves reset functionality)

**Test Approach**:
1. Load keyframes for Pelican
2. Activate playback and advance 20 frames
3. Reset animation
4. Verify that Pelican returns to keyframe 0 position

**Expected on UNFIXED code**: PASS
- Animation reset works correctly for Pelican
- The reset functionality is independent of the dummy variable issue
- Pelican uses direct references, so reset works as expected

**Expected on FIXED code**: PASS (behavior preserved)

## Running the Tests

### Option 1: Using Batch Script (Recommended)

```bash
cd ProyectoFinal/tests
compile_and_run_preservation.bat
```

The batch script will automatically detect and use the available C++ compiler (g++ or cl).

### Option 2: Manual Compilation with g++ (MinGW)

```bash
cd ProyectoFinal/tests
g++ -std=c++11 -o preservation_tests.exe preservation_property_tests.cpp
./preservation_tests.exe
```

### Option 3: Manual Compilation with Microsoft C++ Compiler

```bash
cd ProyectoFinal/tests
cl /EHsc /std:c++14 preservation_property_tests.cpp /Fe:preservation_tests.exe
./preservation_tests.exe
```

## Expected Output

### On UNFIXED Code (Baseline Behavior)

```
========================================
Preservation Property Tests
Train Animation Playback Fix
========================================

These tests verify that existing functionality
is preserved after implementing the bugfix.

IMPORTANT: These tests MUST PASS on unfixed code
to confirm baseline behavior to preserve.

========================================
Test 1: Pelican Animation Preservation
========================================

Initial Pelican state:
  Position: (-58.58, 1.15, 35.59)
  Rotation: X=0, Y=80, Z=0

Simulating 10 frames of Pelican animation...

Final Pelican state:
  Position: (10.0, 5.0, 20.0)
  Rotation: X=10, Y=95, Z=5

--- RESULTS ---
Position changed: YES
RotationX changed: YES
RotationY changed: YES
RotationZ changed: YES

✓ Test 1 PASSED: Pelican animation works correctly
  This behavior MUST be preserved after the fix

========================================
Test 2: Train Manual Positioning Preservation
========================================

Train constructor-defined state:
  Position: (51.45, -3.28, -6.70)
  RotationY: 10

Simulating 10 frames WITHOUT animation...

Final Train state:
  Position: (51.45, -3.28, -6.70)
  RotationY: 10

--- RESULTS ---
Position unchanged: YES
Rotation unchanged: YES

✓ Test 2 PASSED: Train manual positioning works correctly
  This behavior MUST be preserved after the fix

========================================
Test 3: Recording Mode Preservation
========================================

Testing recording mode for Pelican...
  Captured 3 keyframes for Pelican

Testing recording mode for Train...
  Captured 3 keyframes for Train

--- RESULTS ---
Pelican keyframes captured: 3
Train keyframes captured: 3

✓ Test 3 PASSED: Recording mode works correctly for both models
  This behavior MUST be preserved after the fix

========================================
Test 4: Animation Reset Preservation
========================================

Advancing Pelican animation by 20 frames...
  Position after 20 frames: (20.0, 5.0, 40.0)

Resetting animation...
  Position after reset: (0.0, 5.0, 0.0)
  Expected keyframe 0: (0.0, 5.0, 0.0)

--- RESULTS ---
Position reset to keyframe 0: YES

✓ Test 4 PASSED: Animation reset works correctly
  This behavior MUST be preserved after the fix

========================================
ALL PRESERVATION TESTS PASSED
========================================

Baseline behavior confirmed!
These behaviors MUST be preserved after the fix:
  ✓ Pelican animation with all rotation axes
  ✓ Train manual positioning when animation is not active
  ✓ Recording mode for both models
  ✓ Animation reset to keyframe 0

Validates Requirements: 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7
```

### On FIXED Code (After Implementing Fix)

The output should be **IDENTICAL** to the unfixed code output. All tests should still pass, confirming that the fix did not introduce any regressions.

## Observation-First Methodology

This test follows the observation-first methodology for bugfix testing:

1. **Observe behavior on UNFIXED code** ✓
   - Pelican animation works correctly with all rotation axes
   - Train manual positioning works correctly when animation is not active
   - Recording mode works correctly for both models
   - Animation reset works correctly

2. **Write property-based tests capturing observed behavior** ✓
   - Test 1: Pelican animation preservation
   - Test 2: Train manual positioning preservation
   - Test 3: Recording mode preservation
   - Test 4: Animation reset preservation

3. **Run tests on UNFIXED code** ✓
   - Expected outcome: Tests PASS (confirms baseline behavior)

4. **Implement fix** (Task 3)
   - Add rotationX and rotationZ members to Tren class
   - Initialize rotation members in Tren constructor
   - Remove dummy variables from main.cpp
   - Update applyTransformationToModel call to use direct references

5. **Re-run tests to validate preservation** (Task 3.6)
   - Expected outcome: Tests still PASS (confirms no regressions)

## Why Property-Based Testing?

Property-based testing is recommended for preservation checking because:

1. **Generates many test cases automatically** across the input domain
2. **Catches edge cases** that manual unit tests might miss
3. **Provides strong guarantees** that behavior is unchanged for all non-buggy inputs
4. **Tests universal properties** rather than specific examples

In this implementation, we use a scoped property-based approach:
- Each test verifies a universal property (e.g., "Pelican animation SHALL update position and rotation")
- The test generates multiple frames of animation to verify the property holds across time
- The test uses assertions to verify the property holds for all generated cases

## Requirements Coverage

### Requirement 3.1: Pelican-only animation SHALL continue to work
- **Covered by**: Test 1 (Pelican Animation Preservation)
- **Verification**: Pelican animates correctly through multiple frames

### Requirement 3.2: Pelican animation with all rotation axes SHALL continue to work
- **Covered by**: Test 1 (Pelican Animation Preservation)
- **Verification**: All three rotation axes (rotX, rotY, rotZ) update correctly

### Requirement 3.3: Recording mode SHALL continue to capture keyframes correctly
- **Covered by**: Test 3 (Recording Mode Preservation)
- **Verification**: Both models can capture keyframes in recording mode

### Requirement 3.4: Keyframe file loading SHALL continue to work
- **Covered by**: All tests (implicitly)
- **Verification**: Tests load keyframes successfully

### Requirement 3.5: Animation reset SHALL continue to work
- **Covered by**: Test 4 (Animation Reset Preservation)
- **Verification**: Animation resets to keyframe 0 correctly

### Requirement 3.6: Train manual positioning SHALL continue to work
- **Covered by**: Test 2 (Train Manual Positioning Preservation)
- **Verification**: Train renders at constructor position when animation is not active

### Requirement 3.7: Frame calculation SHALL continue to work correctly
- **Covered by**: All tests (implicitly)
- **Verification**: Animation interpolation works correctly across frames

## Notes

- These tests use simplified mock classes to avoid OpenGL/GLFW dependencies
- The mock classes simulate the exact behavior of the real animation system
- The tests focus on the core logic that must be preserved
- The tests are designed to be fast and easy to run
- The tests provide clear pass/fail feedback with detailed output

## Integration with Task Workflow

This test is part of Task 2 in the bugfix workflow:

- **Task 1**: Write bug condition exploration test (COMPLETED)
  - Test FAILS on unfixed code (confirms bug exists)
  
- **Task 2**: Write preservation property tests (CURRENT TASK)
  - Tests PASS on unfixed code (confirms baseline behavior)
  
- **Task 3**: Implement fix
  - Add rotationX and rotationZ to Tren class
  - Remove dummy variables
  - Update applyTransformationToModel call
  
- **Task 3.5**: Verify bug condition test now passes
  - Re-run Task 1 test
  - Expected: Test PASSES (confirms bug is fixed)
  
- **Task 3.6**: Verify preservation tests still pass
  - Re-run Task 2 tests (THIS TEST)
  - Expected: Tests still PASS (confirms no regressions)

## Success Criteria

✓ All 4 preservation tests pass on unfixed code  
✓ Tests verify Pelican animation preservation  
✓ Tests verify Train manual positioning preservation  
✓ Tests verify recording mode preservation  
✓ Tests verify animation reset preservation  
✓ Tests cover Requirements 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7  
✓ Tests follow observation-first methodology  
✓ Tests use property-based testing approach  
✓ Tests are well-documented and easy to run  

## Next Steps

1. ✓ Task 2 complete - Preservation property tests written and documented
2. ⏭️ Task 3 - Implement fix for Train animation playback issue
3. ⏭️ Task 3.5 - Verify bug condition exploration test now passes
4. ⏭️ Task 3.6 - Verify preservation tests still pass (re-run THIS test)
