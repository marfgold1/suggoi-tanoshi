import os
from pathlib import Path

from plz.constants import PLZ_REPO_DIR, PLZ_REPO_OBJ, PLZ_REPO_HEAD

class Project:
    __ins = None

    def __init__(self, create=False):
        self.path: Path = Project.get_config_folder(Path(os.getcwd))
        if not self.path:
            if create:
                self.path = Path(os.getcwd())
            else:
                raise Exception("not a plz repository (or any of the parent directories): .plz")

        # Create repo dir
        self.config_path = self.path.joinpath(PLZ_REPO_DIR)
        self.config_path.mkdir(parents=True, exist_ok=True)

        # Create repo object dir
        self.obj_path = self.config_path.joinpath(PLZ_REPO_OBJ)
        self.obj_path.mkdir(parents=True, exist_ok=True)

        # Read head
        self.head = None
        head_path = self.config_path.joinpath(PLZ_REPO_HEAD)
        if head_path.exists():
            self.head = head_path.read_text()

    @staticmethod
    def get(create=False):
        if Project.__ins:
            return Project.__ins
        return Project(create)

    @staticmethod
    def get_config_folder(path: Path) -> Path | None:
        while path.parent != path:
            if Path(path, Path(".plz")).exists():
                return path
            path = path.parent
        return None
