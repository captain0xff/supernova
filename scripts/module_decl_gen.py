import pathlib

ROOT_PATH = pathlib.Path(__file__).parent.parent
FILEPATH = "include/supernova/core.h"

file = open(ROOT_PATH.joinpath(FILEPATH))
data = file.read()
file.close()

lines = data.splitlines()
for line in lines:
    if (line.startswith("class ") and not line.endswith(";")):
        name = line.split(" ")[1]
        print(f"export using {name};");
