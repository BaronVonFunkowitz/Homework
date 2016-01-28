import random

diceNum=1
minVal=1
maxVal=4

#This variable is for all user input
temp=''

#This rolls the dice
def Roll(dice, minimum, maximum):
    for i in range(dice):
        print('Die #' + str(i+1) + ' rolled '+str(random.randint(minimum,maximum)))
        print('')
        print('')
        
#This validates user input to make sure it's within range and is a number
def getInput(menu):
    good=False
    while good==False:
        choice=input('>')

        #This will make sure it's a number
        try:
            choice=int(choice)
        except ValueError:
            print('Enter a whole numer')
            continue

        #This makes sure if it's in range (if applicable)
        if choice<=0:
            print('You need a value greater than 0')
            continue

        if menu==True:
            if choice>=4:
                print('You chose something outside the menu range')
                continue
            else:
                break
        else:
            break

    return choice


#This is the main program
leave=False
while leave!=True:
    print('Dice Roller')
    print('----------------')
    print('')
    print('1.Roll')
    print('2.Change dice values')
    print('3.Quit')
    print('           Values: Number of Dice ['+str(diceNum)+']')
    print('                   minimum value  ['+str(minVal)+']')
    print('                   maximum value  ['+str(maxVal)+']')
    print('')
    choice=getInput(True)
    
    if choice==1:
        Roll(diceNum,minVal,maxVal)
    elif choice==2:
        print('Num of dice?')
        diceNum=getInput(False)
        print('Min value?')        
        minVal=getInput(False)
        print('Max value?')
        maxVal=getInput(False)
    elif choice==3:
        leave=True
