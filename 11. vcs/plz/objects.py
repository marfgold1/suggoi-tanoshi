from abc import ABC, abstractmethod
from datetime import datetime
import gzip
from hashlib import sha1
from queue import Queue
from typing import List

from plz.project import Project

class PlzObject(ABC):
    def __init__(self):
        self.is_dirty = True
        self.obj_path = Project.get().obj_path

    @classmethod
    def get(cls, _hash: str, to_str=True, **kwargs):
        with gzip.open(Project.get().obj_path.joinpath(_hash), "rb") as f:
            obj:PlzObject = cls.deserialize(f.read().decode() if to_str else f.read(), **kwargs)
        obj.is_dirty = False

    def save(self):
        if self.is_dirty:
            data = self.serialize()
            with gzip.open(self.obj_path.joinpath(self._hash), "wb") as f:
                f.write(data.encode() if isinstance(data, str) else data)
            self.is_dirty = False

    @property
    def _hash(self) -> str:
        return sha1(self.serialize()).hexdigest()

    @abstractmethod
    @classmethod
    def deserialize(cls, data: bytes|str, **kwargs):
        pass

    @abstractmethod
    def serialize(self) -> bytes|str:
        pass

class Blob(PlzObject):
    def __init__(self, name: str, data: bytes):
        super().__init__()
        self.name = name
        self.data = data
        self.obj_type = "blob"

    def serialize(self):
        return self.data

    @classmethod
    def deserialize(cls, data: bytes, **kwargs):
        if 'name' not in kwargs:
            raise Exception("Blob must have a name!")
        return cls(kwargs['name'], data)

    @classmethod
    def get(cls, _hash: str, **kwargs):
        return super().get(_hash, to_str=False, **kwargs)


class Tree(PlzObject):
    def __init__(self, name: str, childrens):
        super().__init__()
        self.name: str = name
        self.childrens: TreeBlob = childrens
        self.obj_type = "tree"

    def serialize(self):
        return "\n".join([
            f"{child.obj_type} {child.name} {child._hash}"
            for child in self.childrens
        ])

    @classmethod
    def deserialize(cls, data: str, **kwargs):
        if 'name' not in kwargs:
            raise Exception("Tree must have a name!")
        childrens = []
        for line in data.split("\n"):
            obj_type, name, _hash = line.split(" ")
            if obj_type not in ["tree", "blob"]:
                raise Exception(f"Invalid object type '{obj_type}' of {name} ({_hash})!")
            childrens.append(
                Tree.get(_hash, name=name)
                if obj_type == "tree" else
                Blob.get(_hash, name=name),
            )
        return cls(kwargs["name"], childrens)

class Commit(PlzObject):
    def __init__(self, tree: str, parent: str, author: str, date: datetime, message: str):
        super().__init__()
        self.tree = tree
        self.parent = parent
        self.message = message
        self.author = author
        self.date = date
        self.obj_type = "commit"

    def serialize(self):
        return "\n".join([
            f"tree {self.tree}"
            f"parent {self.parent}",
            f"author {self.author}",
            f"date {self.date.isoformat()}",
            f"{self.message}",
        ])

    @classmethod
    def deserialize(cls, data: str, **kwargs):
        tree, parent, author, date, message = data.split("\n")
        return cls(
            tree.split(" ")[1],
            parent.split(" ")[1],
            "".join(author.split(" ")[1:]),
            datetime.fromisoformat("".join(date.split(" ")[1:])),
            message
        )
    
    @staticmethod
    def get_head():
        head = Project.get().head
        return Commit.get(head) if head else None

    @staticmethod
    def create():
        proj_path = Project.get().config_path
        head:Commit|None = Commit.get_head()
        if head:
            old_tree = Tree.get(head.tree)
            if not old_tree:
                raise Exception("Tree not found!")
        
        trees = Queue()
        while not trees.empty():
            tree = trees.get()
            for child in tree.childrens:
                if isinstance(child, Tree):
                    trees.put(child)
                else:
                    child.save()

        new_tree = Tree(f"root", [

        ])

TreeBlob = List[Tree|Blob]
