import subprocess
import os
import difflib

EXECUTABLE = './tinker_executable'  # Change to your program
TEST_DIR = './tests'

def run_test(test_file, expected_file):
    # Run the simulator with the test file
    result = subprocess.run(
        [EXECUTABLE, test_file],
        capture_output=True,
        text=True
    )

    # Read expected output
    with open(expected_file, 'r') as f:
        expected_output = f.read()

    # Compare
    if result.stdout.strip() == expected_output.strip():
        print(f"[PASS] {test_file}")
        return True
    else:
        print(f"[FAIL] {test_file}")
        print("=== Expected ===")
        print(expected_output)
        print("=== Got ===")
        print(result.stdout)
        print("=== Diff ===")
        for line in difflib.unified_diff(
            expected_output.splitlines(), result.stdout.splitlines(), lineterm=''
        ):
            print(line)
        return False

def main():
    test_files = sorted(f for f in os.listdir(TEST_DIR) if f.startswith("test_"))
    total = passed = 0
    for test_file in test_files:
        test_path = os.path.join(TEST_DIR, test_file)
        base = test_file.replace("test_", "").replace(".txt", "")
        expected_file = f"expected_{base}.out"
        expected_path = os.path.join(TEST_DIR, expected_file)

        if not os.path.exists(expected_path):
            print(f"[SKIP] No expected output for {test_file}")
            continue

        total += 1
        if run_test(test_path, expected_path):
            passed += 1

    print(f"\nPassed {passed}/{total} tests.")

if __name__ == "__main__":
    main()
