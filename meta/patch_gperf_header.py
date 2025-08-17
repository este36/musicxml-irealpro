import sys

header_path = sys.argv[1]

with open(header_path, "r") as f:
    lines = f.readlines()

with open(header_path, "w") as f:
    for line in lines:
        # Remplace la déclaration de la fonction par static inline ...
        if line.strip().startswith("const struct keyword *"):
            line = line.replace("const struct keyword *", "static inline const struct keyword *")
        elif line.strip().startswith("static unsigned int"):
            line = line.replace("static unsigned int", "static inline unsigned int")
        f.write(line)