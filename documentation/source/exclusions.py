import os
import glob

# Find all files matching _source.md in the site directory
docs_dir = os.getcwd() + "/site/assets/.doxy/"
pattern = os.path.join(docs_dir, "**", "*_8h*.md")
files = glob.glob(pattern, recursive=True)

exclude_string = """---
search:
  exclude: true
---
"""

print("Excluding the following files from search:")
for file in files:
    short_file = os.path.relpath(file, docs_dir)
    print("    - ", short_file)

    # Attach exclusion = true at the top of the file
    with open(file, "r") as f:
        content = f.read()
    content = exclude_string + content
    with open(file, "w") as f:
        f.write(content)
