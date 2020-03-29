import Remove
import Write
from pathlib import Path

if __name__ == "__main__":
    for path in Path('.').rglob('*.cpp'):
        print("Cleaning %s" % path.name)
        
        with open(path, "r+") as fs:
            lines = fs.readlines()
            fs.seek(0)
            print("Result: %s" % "".join(Remove.clearRedundantComments(lines)))
            fs.write("".join(Remove.clearRedundantComments(lines)))
            fs.truncate()