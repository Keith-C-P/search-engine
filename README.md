# Search Engine

A simple search engine that takes a file path as command-line argument and lets you filter line wise.\
The default 'data.txt' is a file that contains around 372,000 lines of sites from Wikipedia

## How to run:

cd into the repo and run: \
`make`
to compile the application, then run: \
`make run`

If you want to regenerate the 'data.txt' file then run:\
`python3 -m venv .venv`
`pip install -r requirements.txt`
then run:\
Linux - `source .venv/bin/activate`
Windows - `.venv\bin\activate.bat`
\to activate the virtual environment

lastly run:\
`python3 main.py`
to generate the data.txt, feel free to change the code in main.py to adjust depth and source_site
