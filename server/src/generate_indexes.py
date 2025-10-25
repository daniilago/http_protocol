import os
from datetime import datetime

BASE_DIR = "../files"

HTML_TEMPLATE = """<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>Content of {title}</title>
<style>
body {{ font-family: Arial, sans-serif; margin: 40px; }}
h1 {{ color: #333; }}
ul {{ list-style: none; padding: 0; }}
li {{ margin: 8px 0; }}
a {{ text-decoration: none; color: #007bff; }}
a:hover {{ text-decoration: underline; }}
footer {{ margin-top: 30px; font-size: 12px; color: #777; }}
</style>
</head>
<body>
<h1>Content of /{title}</h1>
<ul>
{links}
</ul>
<footer>Updated on {date}</footer>
</body>
</html>
"""


def generate_folder_index(folder_path):
    """Generate index.html inside a subfolder listing its files."""
    entries = sorted(os.listdir(folder_path))
    files = [
        name
        for name in entries
        if os.path.isfile(os.path.join(folder_path, name)) and name != "index.html"
    ]

    if not files:
        return None

    links = "\n".join([f'<li><a href="{name}">{name}</a></li>' for name in files])

    html = HTML_TEMPLATE.format(
        title=os.path.basename(folder_path),
        links=links,
        date=datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
    )

    index_path = os.path.join(folder_path, "index.html")
    with open(index_path, "w") as f:
        f.write(html)


def generate_main_index():
    """Generate the main index.html inside 'files' listing subfolders."""
    subfolders = [
        name
        for name in os.listdir(BASE_DIR)
        if os.path.isdir(os.path.join(BASE_DIR, name))
    ]

    links = "\n".join(
        [
            f'<li><a href="{name}/index.html">{name}/</a></li>'
            for name in sorted(subfolders)
        ]
    )

    html = HTML_TEMPLATE.format(
        title=BASE_DIR[3:],
        links=links,
        date=datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
    )

    index_path = os.path.join(BASE_DIR, "index.html")
    with open(index_path, "w") as f:
        f.write(html)


def main():
    # Generate index for each subfolder
    for name in os.listdir(BASE_DIR):
        folder_path = os.path.join(BASE_DIR, name)
        if os.path.isdir(folder_path):
            generate_folder_index(folder_path)

    # Generate main index for the 'files' directory
    generate_main_index()


if __name__ == "__main__":
    main()
