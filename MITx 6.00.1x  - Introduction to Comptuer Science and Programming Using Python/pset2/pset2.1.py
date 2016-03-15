balance = 4842
annualInterestRate = 0.2
monthlyPaymentRate = 0.04
	      
totalPay = 0
prevBal = balance
monthlyInterestRate = annualInterestRate/12.0

for i in range(1,13):
    minimumPay = monthlyPaymentRate * prevBal
    totalPay += minimumPay
    monthlyUnpaidBal = prevBal - minimumPay
    updatedBal = monthlyUnpaidBal + monthlyInterestRate * monthlyUnpaidBal
    prevBal = updatedBal

    print "Month: " + str(i)
    print "Minimum monthly payment: " + str(round(minimumPay,2))
    print "Remaining balance: " + str(round(updatedBal,2))
    
print "Total paid: " + str(round(totalPay,2))
print "Remaining balance: " + str(round(updatedBal,2))