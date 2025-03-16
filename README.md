# Search Engine

A simple search engine that takes a file path as command-line argument and lets you filter line wise.\
Uses the **ncurses** library for terminal handling, so you may need to install ncurses in windows, usually its preinstalled in linux


The default 'data.txt' is a file that contains around 372,000 lines of sites from Wikipedia\
If you wish to regenerate the data.txt file refer to the [Regenerate Data](#regen_data) section

## How to run:

In the 'Makefile' **please** change the `file_location` to the file location of your own 'data.txt', it should be in the root of the repository

cd into the repository and run: \
`make`\
to compile the application, then run: \
`make run`

<details open> 
<summary>
  <a name="regen_data">
  <h3>Regenerate Data</h3>
  </a>
</summary>

To regenerate the 'data.txt' file run:\
`python3 -m venv .venv`

Then run either of these to activate the venv:\
Linux - `source .venv/bin/activate`\
Windows - `.venv\bin\activate.bat`

Then run this to install dependencies:\
`pip install -r requirements.txt`

Lastly run this to generate the 'data.txt'\:\
`python3 main.py`

Feel free to change the code in main.py to adjust **DEPTH** and **SOURCE_SITE**

> Run this to stop the venv\
> ```deactivate```
</details>
