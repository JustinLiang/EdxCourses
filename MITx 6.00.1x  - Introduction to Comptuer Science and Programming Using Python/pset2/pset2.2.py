balance = 3200000000
annualInterestRate = 0.2

episilon = 0.0005
prevBal = balance
updatedBal = balance
monthlyInterestRate = annualInterestRate/12.0
lowerBound = balance/12
upperBound = (balance * (1+monthlyInterestRate) ** 12) / 12.0
minimumPay = (lowerBound + upperBound)/2

while abs(updatedBal) > episilon:
    prevBal = balance
    updatedBal = balance
    for i in range(1,13):
        monthlyUnpaidBal = prevBal - minimumPay
        updatedBal = monthlyUnpaidBal + monthlyInterestRate * monthlyUnpaidBal
        prevBal = updatedBal
    if updatedBal > 0:
        lowerBound = minimumPay
    else:
        upperBound = minimumPay
    minimumPay = (lowerBound + upperBound)/2
print "Lowest Payment: " + str(round(minimumPay,2))
print "Final Balance: " + str(updatedBal)