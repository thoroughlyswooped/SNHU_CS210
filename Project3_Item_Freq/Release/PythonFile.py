import re
import string
from os.path import exists

# Dictionary to store items and their frequencies instead of creating a seperate file
word_freq_dict = {}
file_written = False;
file_name = ""

# NOTE(Ezra): these were left here because do not have enough information to justify removing them
#region OldFunctions
def printsomething():
    print("Hello from python!")

def PrintMe(v):
    print("You sent me: " + v)
    return 100;

def SquareValue(v):
    return v * v

def DoubleValue(v):
    return 2 * v
#endregion

# Takes the name of an item and returns how many times it appeares in the input file
# Return -1 if not found
def GetFreq(s_name):
    if s_name in word_freq_dict:
        return word_freq_dict[s_name]
    else:
        return -1

# Make a dictionary from the input file so we can use it later
# If there is not a data file, write the dictionary there
def MakeDict():
    # Clear Dictionary just in case
    global word_freq_dict
    word_freq_dict = {}

    # read in file
    with open('CS210_Project_Three_Input_File.txt') as f:
        lines = f.readlines()
        for line in lines:
            line = line.strip()             # Remove the trailing and leading characters (i.e. \n)
            if (line in word_freq_dict):
                word_freq_dict[line] += 1
            else:
                word_freq_dict[line] = 1

    # if we have not already created the file, create it now.
    file_exists = exists(file_name)
    if not file_exists:
        WriteDictionaryToFile()
    return 0;

# Set the name of the file that we want to write to
#NOTE(Ezra) this does not delete files if we are now using a new name
def SetDataFile(s_name):
    global file_name
    file_name = s_name
    return 0

def WriteDictionaryToFile():
    global file_written

    if file_written:
        return
    f = open(file_name, "w")
    for key, value in word_freq_dict.items():
        f.write("{} : {}\n".format(key, value))
    f.close()
    file_written = True

def ShowFrequencies():
    print("Frequency Of Items\n" + 
          "------------------")
    for key, value in word_freq_dict.items():
        s = "{} : {}".format(key, value)
        print(s)
    print()