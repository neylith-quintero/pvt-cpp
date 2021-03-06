
begin_correlation("Tpr", "GasPseudoreducedTemperature", "PseudoReducedTemperature") 
add_title("CALCULATION OF THE PSEUDOREDUCED TEMPERATURE")
add_note("The temperature is expressed as a ratio of its critical value.")
add_ref("ahmed:1989")
# add_ref("banzer:1996") Secondary reference
add_parameter("t", "Rankine", "Temperature")
add_parameter("tpc", "Rankine", "Gas pseudocritical temperature")
add_author("Standard Equation")
end_correlation()

################################################################

begin_correlation("Ppr", "GasPseudoreducedPressure", "PseudoReducedPressure") 
add_title("CALCULATION OF THE PSEUDOREDUCED PRESSURE")
add_note("The pressure is expressed as a ratio of its critical value.")
add_ref("ahmed:1989")
# add_ref("banzer:1996") Secondary reference
add_parameter("p", "psia", "Pressure")
add_parameter("ppc", "psia", "Gas pseudocritical pressure")
add_author("Standard Equation")
end_correlation()
