from typing import *
import random as rand
import re
import os
import subprocess

class Bible:
    def __init__(self):
        with open("./assets/Bible.txt") as bible:
            self.text = bible.read()
            self.text = re.sub(r'[\n\t\r]', ' ', self.text)

    def getSpan(self, l: int) -> str:
        id = rand.randint(0, len(self.text) - l)
        return self.text[id:id+l]

def run(pattern: str, mod: int, prime: int) -> List[str]:
    out: List[str] = []
    for root, _, files in os.walk('src'):
        rand.shuffle(files)
        for file in files:
            if not file.endswith('.py'):
                continue
            print(f"\t\t{root}/{file}")
            cmd = ['python', os.path.join(root, file), pattern, f'{mod}', f'{prime}']
            output = subprocess.run(cmd, capture_output=True)
            out.append(output.stdout.decode('utf-8'))
    return out





def main() -> None:
    bible = Bible()
    sizes = [2 ** i for i in range(0, 16)]
    sampleRate = 12
    prime = 931097 

    with open('report.csv', 'w') as out:
        out.write('')
        for j, size in enumerate(sizes):
            for i in range(sampleRate):
                print(f"BATCH #{i+1}/{sampleRate}:  it {j+1}/{len(sizes)}, patternSize={size}")
                out.writelines(run(bible.getSpan(size), prime, 256))

if __name__ == '__main__':
    main()
