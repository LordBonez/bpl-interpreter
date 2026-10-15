"""Compare program output against the original course fixtures."""
import argparse
import difflib
from pathlib import Path
import re
import subprocess
import sys

def normalized(text):
    # Platform line endings, trailing spaces and blank lines are cosmetic.
    return [line.rstrip() for line in text.splitlines() if line.strip()]

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('executable', type=Path)
    args = parser.parse_args()
    executable = args.executable.resolve()
    fixtures = Path(__file__).parent / 'fixtures'
    inputs = sorted((p for p in fixtures.glob('testprog*') if re.fullmatch(r'testprog\d+', p.name)),
                    key=lambda p: int(re.search(r'\d+', p.name).group()))
    if not inputs:
        parser.error('No fixture inputs found')
    passed = 0
    original_matches = 0
    for path in inputs:
        try:
            result = subprocess.run([str(executable), str(path.resolve())], capture_output=True,
                                    text=True, timeout=10)
            actual = normalized(result.stdout + result.stderr)
            original = normalized(path.with_suffix('.correct').read_text())
            erratum = path.with_suffix('.verified.correct')
            expected = normalized(erratum.read_text()) if erratum.exists() else original
            good = result.returncode == 0 and actual == expected
            original_matches += result.returncode == 0 and actual == original
            passed += good
            note = ' (documented reference erratum)' if erratum.exists() else ''
            print(f'{"PASS" if good else "FAIL"} {path.name}{note}')
            if not good:
                print('\n'.join(difflib.unified_diff(expected, actual, fromfile='expected', tofile='actual', lineterm='')))
        except subprocess.TimeoutExpired:
            print(f'FAIL {path.name}: timed out')
    print(f'\n{passed}/{len(inputs)} project expectations matched.')
    print(f'{original_matches}/{len(inputs)} unmodified course reference outputs matched.')
    return 0 if passed == len(inputs) else 1

if __name__ == '__main__':
    sys.exit(main())
