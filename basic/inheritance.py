class Fish:
    def __init__(self, first_name, last_name="Fish"):
        self.first_name = first_name
        self.last_name = last_name

    def swim(self):
        print("The fish is swimming.")

    def swim_backwards(self):
        print("The fish can swim backwards.")


class Clownfish(Fish):

    def live_with_anemone(self):
        print("The clownfish is coexisting with sea anemone.")


if __name__ == "__main__":

    nemo = Clownfish("Nemo")
    nemo.swim()
    nemo.swim_backwards()
    nemo.live_with_anemone()

    