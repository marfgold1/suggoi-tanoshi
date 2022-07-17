import argparse
import os
from pathlib import Path

class Commit(object):
    def __init__(self, hash, message, author, date):
        self.hash = hash
        self.message = message
        self.author = author
        self.date = date
    
    @classmethod
    def create(project):
        pass

    @classmethod
    def current(project):
        pass

class Project:
    def __init__(self):
        self.path: Path = Project.get_config_folder(Path(os.getcwd))
        if not self.path:
            raise Exception("No config folder found")
        self.current_commit = Commit.current(self)

    @staticmethod
    def get_config_folder(path: Path) -> None:
        while path.parent != path:
            if Path(path, Path(".plz")).exists():
                return path
            path = path.parent
        return None
