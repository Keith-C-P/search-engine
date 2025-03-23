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

Feel free to change the code in `main.py` to adjust **DEPTH** and **SOURCE_SITE**

Linux:
```
python3 -m venv .venv           # ╮
source .venv/bin/activate       # ├ venv creation  
pip install -r requirements.txt # ╯
python3 main.py                 # ├ regenerating the data
```
Windows:
```
python3 -m venv .venv           # ╮
.venv\bin\activate.sh           # ├ venv creation  
pip install -r requirements.txt # ╯
python3 main.py                 # ├ regenerating the data
```


> Run this to stop the venv\
> ```deactivate```
</details>
