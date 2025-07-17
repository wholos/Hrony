import os, osproc, strutils

for arg in commandLineParams():
  case arg
    of "--help":
      echo "Install package - install"
      echo "Remove package - delete"
      quit(0)
    of "install":
      echo "Install..."
      quit(0)
    of "delete":
      echo "Delete..."
      quit(0)
    else:
      echo "Unknown argument: ", arg
      quit(0)

echo "Enter argument, arguments in --help."
