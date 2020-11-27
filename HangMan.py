import random

class HangMan:
    def __init__(self):
        self.__words = ["hello", "apple", "level", "friends"]
    
    def addWord(self, word):
        self.__words.append(word.lower())
        print(f"'{word}' added.")

    def startGame(self):
        question = random.choice(self.__words)
        wrong_letters = []
        right_letters = []
        guess_word = [ '_' for i in range(len(question))]

        print("Hangman\nGuess the word within 10 wrong attempts\n\n")
        while len(wrong_letters) < 10 and '_' in guess_word:
            print("Word: ", end='')
            for x in guess_word:
                print(x, end='')
            print('\n')
            letter = input("Guess letter: ")
            letter = letter.lower()
            if len(letter) == 1 and letter.isalpha():
                if letter not in wrong_letters and letter not in right_letters:
                    if letter in question:
                        for i in range(len(question)):
                            if question[i] == letter:
                                guess_word[i] = letter
                                right_letters.append(letter)
                    else:
                        wrong_letters.append(letter)
                        print(f"Wrong letter. {10 - len(wrong_letters)} attempts remaining.")
                else:
                    print("Letter already tried.")
            else:
                print("Wrong input.")
            
        if len(wrong_letters) == 10:
            print("Game lost.")
            return
        print(f"Word found was {question}")

if __name__ == "__main__":
    game = HangMan()
    game.startGame()