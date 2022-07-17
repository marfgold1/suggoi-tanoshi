from configparser import ConfigParser
from pathlib import Path
from plz.project import Project

class Plz(object):
    def __init__(self, create=False):
        self.project = Project(create)
        global_conf_dir = Path.home().joinpath(".plz")
        global_conf_dir.mkdir(parents=True, exist_ok=True)
        conf_file = global_conf_dir.joinpath(".plz.conf")
        conf_file.touch(exist_ok=True)
        self.global_conf = ConfigParser()
        self.global_conf.read_string(conf_file.read_text())

    def commit(self):
        # Check if commit author exist
        if (
            self.global_conf.has_option("Commit", "AuthorName")
            and self.global_conf.has_option("Commit", "AuthorEmail")
        ):
            author = (
                self.global_conf.get("Commit", "Author") +
                " <" + self.global_conf.get("Commit", "AuthorEmail") + ">"
            )
        else:
            raise Exception("No commit author found!")

    @staticmethod
    def create(self):
        pass
