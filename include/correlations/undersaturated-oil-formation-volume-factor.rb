# coding: utf-8
begin_correlation("BoaMcCain", "UndersaturatedOilVolumeFactor", "RB_STB")
add_title("McCAIN CORRELATION, CALCULATION OF UNDERSATURATED OIL FORMATION VOLUME FACTOR")
add_db("Based on the equation presented by McCain (1990).")
add_db("The volume factor at bubble point pressure is adjusted to higher pressures through the use of the coefficient of isothermal compresibility.")
add_ref("mcCain:1990") 
# add_ref("banzer:1996") Secondary reference         
add_parameter("bobp", "RB_STB", "Oil formation volume factor at Pb")
add_parameter("p", "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure")
add_parameter("coa", "psia_1", "Oil isothermal compressibility")
add_precondition("p", "pb")
add_author("McCain")
add_note("This correlation determines the oil formation volume factor for pressures above the bubble point pressure.")
add_internal_note("The correlation was verified by using the original reference and Bánzer (1996) as a secondary reference. Date: October 26 2016")
end_correlation()

################################################################

## verificada con python

begin_correlation("BoaDeGhetto", "UndersaturatedOilVolumeFactor",
         "RB_STB")
add_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS (VAZQUEZ & BEGGS CORRELATION), CALCULATION OF UNDERSATURATED OIL FORMATION VOLUME FACTOR")
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for °API<=10, heavy oils for 10<°API<=22.3.")
add_parameter("bobp", "RB_STB", "Oil formation volume factor at Pb") 
add_parameter("yg", "Sgg", "Gas specific gravity", 0.623, 1.517)
add_parameter("api", "Api", "API oil gravity", 6, 22.3)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 17.21, 640.25)
add_parameter("t", "Fahrenheit", "Temperature", 131.4, 250.7)
add_parameter("tsep", "Fahrenheit", "Separator temperature", 59, 177.8)
add_parameter("p", "psia", "Pressure", 1038.49, 7411.54)
add_parameter("pb", "psia", "Bubble point pressure")
add_parameter("psep", "psia", "Separator pressure", 14.5, 752.2)
add_precondition("p", "pb")
add_author("De Ghetto, Paone & Villa")
add_ref("deGhetto:1995")
add_internal_note("De Ghetto et al. recommend the use of the Vazquez & Beggs correlation for the estimation of Bo. The values for the coefficients c1, c2, and c3 are selected only for API<=30.")
add_internal_note("McCain's correlation for Boa is evaluated with the Coa value obtained from the De Ghetto, Paone & Villa correlation.")
set_hidden()
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaHanafy", "UndersaturatedOilVolumeFactor",
         "RB_STB")
add_title("HANAFY ET AL. CORRELATION, CALCULATION OF UNDERSATURATED OIL FORMATION VOLUME FACTOR")
add_db("Based on experimental PVT data of 324 fluid samples taken from 176 wells located in 75 fields. This data represents 15 productive zones of 123 reservoirs distributed along three different regions of Egypt, including the Gulf of Suez, Western Desert, and Sinai.")
add_parameter("bobp", "RB_STB", "Oil formation volume factor at Pb") 
add_parameter("p", "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure")
add_precondition("p", "pb")
add_author("Hanafy et al.")
add_ref("hanafy:1997")
add_internal_note("McCain's correlation for Boa is evaluated with the Coa value obtained from the Hanafy et al. correlation.")
set_hidden()
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaKartoatmodjoSchmidt", "UndersaturatedOilVolumeFactor",
         "RB_STB")
add_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF UNDERSATURATED OIL FORMATION VOLUME FACTOR")
add_db("Based on a set of 5392 data points, which represent 740 different crude oil samples.")
add_db("The data bank was collected from PVT reports and literature. The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_parameter("bobp", "RB_STB", "Oil formation volume factor at Pb") 
add_parameter("yg", "Sgg", "Gas specific gravity", 0.4824, 1.668)
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(59)", "Quantity<Api>(14.4)")
add_synonym("yo", "api", "Api")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 0, 2890)
add_parameter("t", "Fahrenheit", "Temperature", 75, 320)
add_parameter("tsep", "Fahrenheit", "Separator temperature", 38, 294)
add_parameter("p", "psia", "Pressure", 24.7, 6014.7)
add_parameter("pb", "psia", "Bubble point pressure")
add_parameter("psep", "psia", "Separator pressure", 14.7, 1414.7)
add_precondition("p", "pb")
add_author("Kartoatmodjo & Schmidt")
add_ref("kartoatmodjo:1991")
add_internal_note("McCain's correlation for Boa is evaluated with the Coa value obtained from the Kartoatmodjo & Schmidt correlation.")
set_hidden()
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaPetroskyFarshad", "UndersaturatedOilVolumeFactor",
         "RB_STB")
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF UNDERSATURATED OIL FORMATION VOLUME FACTOR")
add_db("Based on 81 laboratory PVT analyses. Fluid samples were obtained from reservoirs located offshore Texas and Louisiana.")
add_db("Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_db("Many Gulf of Mexico crudes can be characterized as naphthenic or biodegraded oils.")
add_parameter("bobp", "RB_STB", "Oil formation volume factor at Pb") 
add_parameter("yg", "Sgg", "Gas specific gravity", 0.5781, 0.8519)
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(45)", "Quantity<Api>(16.3)")
add_synonym("yo", "api", "Api")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 217, 1406)
add_parameter("t", "Fahrenheit", "Temperature", 114, 288)
add_parameter("p", "psia", "Pressure", 1700, 10692)
add_parameter("pb", "psia", "Bubble point pressure")
add_precondition("p", "pb")
add_author("Petrosky & Farshad")
add_ref("petrosky:1993")
add_internal_note("McCain's correlation for Boa is evaluated with the Coa value obtained from the Petrosky & Farshad correlation.")
set_hidden()
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaVasquezBeggs", "UndersaturatedOilVolumeFactor",
         "RB_STB")
add_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF UNDERSATURATED OIL FORMATION VOLUME FACTOR")
add_db("Based on more than 600 laboratory PVT analyses from fields all over the world.")
add_parameter("bobp", "RB_STB", "Oil formation volume factor at Pb") 
add_parameter("yg", "Sgg", "Gas specific gravity", 0.511, 1.351)
add_parameter("api", "Api", "API oil gravity", 15.3, 59.5)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 9.3, 2199)
add_parameter("t", "Fahrenheit", "Temperature")
add_parameter("tsep", "Fahrenheit", "Separator temperature", 76, 150)
add_parameter("p", "psia", "Pressure", 141, 9515)
add_parameter("pb", "psia", "Bubble point pressure")
add_parameter("psep", "psia", "Separator pressure", 60, 565)
add_precondition("p", "pb")
add_author("Vazquez & Beggs")
add_ref("vazquez:1980")
add_ref("alShammasi:2001")
# add_ref("banzer:1996") Secondary reference
add_internal_note("McCain's correlation for Boa is evaluated with the Coa value obtained from the Vazquez & Beggs correlation.")
set_hidden()
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaPerezML", "UndersaturatedOilVolumeFactor",
         "RB_STB")
add_title("PÉREZ, HENY & LAGO CORRELATION (EZEKWE-QUERIN-HUMPHREY CORRELATION USED BY PÉREZ, HENY & LAGO), CALCULATION OF UNDERSATURATED OIL FORMATION VOLUME FACTOR")
add_note("The correlation could not be verified because the original reference is not available. Date: August 22 2016.")
add_parameter("bobp", "RB_STB", "Oil formation volume factor at Pb") 
add_parameter("yg", "Sgg", "Gas specific gravity", 0.571, 0.981)
add_parameter("yo", "Sg_do", "Oil specific gravity", "Quantity<Api>(12.9)", "Quantity<Api>(6.4)")
add_synonym("yo", "api", "Api")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 38, 121)
add_parameter("t", "Fahrenheit", "Temperature", 112, 300)
add_parameter("p", "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure", 405, 1335)
add_precondition("p", "pb")
add_author("Pérez, Heny & Lago")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 22 2016.")
add_internal_note("McCain's correlation for Boa is evaluated with the Coa value obtained from the Pérez, Heny & Lago correlation.")
set_hidden()
end_correlation()

################################################################ 

## verificada con python
begin_correlation("BoaMillanArcia", "UndersaturatedOilVolumeFactor",
         "RB_STB")
add_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF UNDERSATURATED OIL FORMATION VOLUME FACTOR")
add_db("Venezuelan heavy crudes were correlated.")
add_parameter("bobp", "RB_STB", "Oil formation volume factor at Pb") 
add_parameter("api", "Api", "API oil gravity", 9.0, 20.2)
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb", 53, 459)
add_parameter("t", "Fahrenheit", "Temperature", 87, 195)
add_parameter("p", "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure", 222.0, 3432.7)
add_precondition("p", "pb")
add_author("Millán-Arcia")
add_ref("millan:1984")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 22 2016.")
add_internal_note("McCain's correlation for Boa is evaluated with the Coa value obtained from the Millán-Arcia correlation.")
set_hidden()
end_correlation()


