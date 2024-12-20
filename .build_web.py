from shutil import copyfile
from subprocess import check_output, CalledProcessError
import sys
import os
import platform
import subprocess

Import("env")

def is_tool(name):
    cmd = "where" if platform.system() == "Windows" else "which"
    try:
        check_output([cmd, name])
        return True
    except:
        return False;

def build_web():
    if is_tool("npm"):
        os.chdir("web")
        print("Attempting to build webpage...")
        try:
            if platform.system() == "Windows":
                print(check_output(["npm.cmd", "install"], text=True))
                print(check_output(["node_modules\\.bin\\gulp.cmd"], text=True))
            else:
                print(check_output(["npm", "install"], text=True))
                print(check_output(["node_modules/.bin/gulp"], text=True))
        except OSError as e:
            print("Encountered error OSError building webpage:", e)
            if e.filename:
                print("Filename is", e.filename)
            print("WARNING: Failed to build web package. Using pre-built page.")
        except CalledProcessError as e:
            print(e.output)
            print("Encountered error CalledProcessError building webpage:", e)
            print("WARNING: Failed to build web package. Using pre-built page.")
        except Exception as e:
            print("Encountered error", type(e).__name__, "building webpage:", e)
            print("WARNING: Failed to build web package. Using pre-built page.")
        finally:
            os.chdir("..");

build_web()