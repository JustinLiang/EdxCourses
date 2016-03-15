print "Please think of a number between 0 and 100!"

low = 0
high = 100

while True:
    guess = (low + high)/2
    guess
    print "Is your secret number " + str(guess) + "?",
    userInput = raw_input("Enter 'h' to indicate the guess is too high. Enter 'l' to indicate the guess is too low. Enter 'c' to indicate I guessed correctly. ")
    if userInput == 'h':
        high = guess
    elif userInput == 'l':
        low = guess
    elif userInput == 'c':
        print "Game over. Your secret number was: " + str(guess)
        break        
    else:
        print "Sorry, I did not understand your input."