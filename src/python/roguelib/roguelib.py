class Player:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.hp = 10
        self.dmg = 1
    
    def move(self, move_input, dungeon_layout):
        new_x = self.x
        new_y = self.y
        if move_input == "w":
            new_y -= 1
        elif move_input == "s":
            new_y += 1
        elif move_input == "d":
            new_x += 1
        elif move_input == "a":
            new_x -= 1
        else:
            print("Unknown input")
            return

        if 0 <= new_y < len(dungeon_layout) and 0 <= new_x < len(dungeon_layout[0]) and dungeon_layout[new_y][new_x] != "1":
            self.x = new_x
            self.y = new_y

class Dungeon:
    def __init__(self):
        self.layout = []
        self.original_layout = []
    
    def convert(self):
        new_layout = []
        for row in self.layout:
            new_row = []
            for item in row:
                new_row.append(str(item))
            new_layout.append(new_row)
        self.layout = new_layout
        self.original_layout = [row[:] for row in self.layout]
    
    def draw(self, player_x, player_y):
        self.layout = [row[:] for row in self.original_layout]
        self.layout[player_y][player_x] = "2"
        for y in range(len(self.layout)):
            for x in range(len(self.layout[0])):
                print(self.layout[y][x].replace("0", ".").replace("1", "#").replace("2", "@"), end="")
            print()

    def flip(self):
        for _ in range(25):
            print()