import os, osproc, strutils, install, delete, update

for arg in commandLineParams():
  case arg
    of "--help":
      echo "Install package - install"
      echo "Remove package - delete"
      echo "Update all package - update"
      quit(0)
    of "install":
      install()
      quit(0)
    of "delete":
      delete()
      quit(0)
    of "update":
      update()
      quit(0)
    else:
      echo "Unknown argument: ", arg
      quit(0)

echo "Enter argument, arguments in --help."
