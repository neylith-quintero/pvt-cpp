# coding: utf-8
begin_correlation("BoaMcCain", "UndersaturatedOilFormationVolumeFactor", "RB_STB")
add_title("McCain CORRELATION, CALCULATION OF UNDERSATURATED OIL FORMATION VOLUME FACTOR")
add_db("Based on the equation presented by McCain (1990).")
add_db("The volume factor at bubble point pressure is adjusted to higher pressures through the use of the coefficient of isothermal compresibility.")
add_ref("mcCain:1990")  
add_ref("banzer:1996")                 
add_parameter("p", "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure")
add_parameter("bobp", "RB_STB", "Oil formation volume factor at pb")
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("McCain")
add_note("This correlation determines the oil formation volume factor for pressures above the bubble point pressure.")
add_internal_note("The correlation was verified by using the original reference and Bánzer (1996) as a secondary reference. Date: October 26 2016")
end_correlation()

################################################################

begin_correlation("BoaAlmarhoun", "OilFormationVolumeFactor", "RB_STB",
                  1.032, 1.997)
add_title("AL-MARHOUN CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 69 bottomhole fluid samples from 69 Middle East oil reservoirs.")
add_ref("alMarhoun:1988")
add_ref("banzer:1996") 
add_ref("mcCain:1990")                  
add_parameter("yg", "Sgg", "Gas specific gravity",  0.752,  1.367)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(44.6)",  "Quantity<Api>(19.4)")
add_synonym("yo", "api", "Api")
add_parameter("rs", "SCF_STB", "Solution GOR",  26,  1602)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Rankine", "Temperature",  "Quantity<Fahrenheit>(74)",  "Quantity<Fahrenheit>(240)") 
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  130,  3573)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Al-Marhoun")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaAlShammasi", "OilFormationVolumeFactor", "RB_STB",
                  1.02, 2.916)
add_title("Al-SHAMMASI CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on a total of 1709 data sets (1661 data sets from 13 different published literature papers and 48 data sets originated from Kuwait reservoirs from unpublished sources).")
add_db("The data available after excluding the duplicates and the repeated data sets for formation volume factor are 1345 data sets.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.51,  3.44)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(63.7)",  "Quantity<Api>(6)")
add_synonym("yo", "api", "Api")
add_parameter("rs", "SCF_STB", "Solution GOR",  6,  3298.6)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  74,  341.6)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  31.7,  7127)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Al-Shammasi")
add_ref("alShammasi:1999")
add_ref("mcCain:1990")
end_correlation()

###############################################################

## verificada con python
begin_correlation("BoaDeGhetto", "OilFormationVolumeFactor", "RB_STB",
                  1.057, 1.362)
add_title("DE GHETTO, PAONE & VILLA CORRELATION FOR EXTRA-HEAVY AND HEAVY OILS (VAZQUEZ & BEGGS CORRELATION), CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 1200 measured data points of 63 heavy and extra-heavy oil samples obtained from the Mediterranean Basin, Africa and the Persian Gulf.")
add_db("Oil samples have been divided in two different API gravity classes: extra-heavy oils for °API<=10, heavy oils for 10<°API<=22.3.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.623,  1.517)
add_parameter("api",  "Api", "API oil gravity",  6,  22.3)
add_parameter("rs", "SCF_STB", "Solution GOR",  17.21,  640.25)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  131.4,  250.7)
add_parameter("tsep", "Fahrenheit", "Separator temperature",  59,  177.8)
add_parameter("p", "psia", "Pressure",  1038.49,  7411.54)
add_parameter("pb", "psia", "Bubble point pressure",  208.86,  4021.96)
add_parameter("psep", "psia", "Separator pressure",  14.5,  752.2)
add_parameter("co", "psia_1", "Coefficient of isothermal compressibility ",  3.02e6,  42.9e6)
add_author("De Ghetto, Paone & Villa")
add_ref("deGhetto:1995")
add_internal_note("De Ghetto et al. recommend the use of the Vazquez & Beggs correlation for the estimation of Bo. The values for the coefficients c1, c2, and c3 are selected only for API<=30.")
end_correlation()

################################################################

begin_correlation("BoaDindorukChristman", "OilFormationVolumeFactor", "RB_STB",
                  1.0845, 2.7381)
add_title("DINDORUK & CHRISTMAN CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on approximately 100 PVT reports from the Gulf of Mexico.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.6017,  1.027)
add_parameter("api",  "Api", "API oil gravity",  14.7,  40)
add_parameter("rs",  "SCF_STB", "Solution GOR")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  133,  3050)
add_parameter("t", "Fahrenheit", "Temperature",  117,  276)
add_parameter("tsep",  "Fahrenheit", "Separator temperature")
add_parameter("p",  "psia", "Pressure")
add_parameter("pb",  "psia", "Bubble point pressure")
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Dindoruk & Christman")
add_ref("dindoruk:2004")
add_ref("mcCain:1990")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaDoklaOsman", "OilFormationVolumeFactor", "RB_STB",
                  1.216, 2.493)
add_title("DOKLA & OSMAN CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 51 bottomhole samples from UAE reservoirs.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.789,  1.29)
add_parameter("yo", "Sg_do", "Oil specific gravity",  0.8236,  0.886)
add_synonym("yo", "api", "Api")
add_parameter("rs",  "SCF_STB", "Solution GOR")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  181,  2266)
add_parameter("t", "Rankine", "Temperature",  "Quantity<Fahrenheit>(190)",  "Quantity<Fahrenheit>(275)") 
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  590,  4640)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Dokla & Osman")
add_ref("dokla:1992")
add_ref("banzer:1996")
add_ref("mcCain:1990")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaGlaso", "OilFormationVolumeFactor", "RB_STB",
                  1.087, 2.588)
add_title("GLASO CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")                
add_db("Based on 26 samples from the North Sea (collected from wells in the region 56 to 62°N) and 19 samples from the Middle East, Algeria, and several areas in the U.S.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.650,  1.276)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(48.1)",  "Quantity<Api>(22.3)")
add_synonym("yo", "api", "Api")
add_parameter("rs", "SCF_STB", "Solution GOR",  90,  2637)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  80,  280)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  150,  7127)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Glaso")
add_ref("glaso:1980")
add_ref("banzer:1996")
add_ref("mcCain:1990")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaHanafy", "OilFormationVolumeFactor", "RB_STB",
                  1.032, 4.35)
add_title("HANAFY ET AL. CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on experimental PVT data of 324 fluid samples taken from 176 wells located in 75 fields. This data represents 15 productive zones of 123 reservoirs distributed along three different regions of Egypt, including the Gulf of Suez, Western Desert, and Sinai.")
add_parameter("rs", "SCF_STB", "Solution GOR",  7,  4272)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  36,  5003)
add_parameter("co", "psia_1", "Coefficient of isothermal compressibility ",  4.46e-6,  62.68e-6)
add_author("Hanafy et al.")
add_ref("hanafy:1997")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaKartoatmodjo", "OilFormationVolumeFactor", "RB_STB",
                  1.007, 2.144)
add_title("KARTOATMODJO & SCHMIDT CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on a set of 5392 data points, which represent 740 different crude oil samples.")
add_db("The data bank was collected from PVT reports and literature. The first major source was from South East Asia, mainly Indonesia. The second source was North America, including the offshore area. The rest came from the Middle East and Latin America.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.4824,  1.668)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(59)",  "Quantity<Api>(14.4)")
add_synonym("yo", "api", "Api")
add_parameter("rs", "SCF_STB", "Solution GOR",  0,  2890)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  75,  320)
add_parameter("tsep", "Fahrenheit", "Separator temperature",  38,  294)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  14.7,  6054.7)
add_parameter("psep", "psia", "Separator pressure",  14.7,  1414.7)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Kartoatmodjo & Schmidt")
add_ref("kartoatmodjo:1991")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaLasater", "OilFormationVolumeFactor", "RB_STB")
add_title("LASATER CORRELATION (STANDING CORRELATION APPLIED FOR LASATER), CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Lasater correlation is based on data obtained from Canada, Western and Mid-Continental United States, and South America.")
add_db("Standing correlation for formation volumes of bubble point liquids is based on data collected from 22 different Californian crude-oil-natural-gas mixtures (Western United States).")
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(51.1)",  "Quantity<Api>(17.9)")
add_synonym("yo", "api", "Api")
add_parameter("rs",  "SCF_STB", "Solution GOR")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  3,  2905)
add_parameter("t", "Fahrenheit", "Temperature",  82,  272)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  48,  5780)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Lasater")
add_ref("lasater:1958")
add_ref("standing:1947")
add_ref("beggs:1987")
add_ref("mcCain:1990")
add_internal_note("Lasater didn't present a correlation for Bo. Standing's correlation is used to calculate Bo because of the similarity of his data bank and development ranges with Lasater's.")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaPetroskyFarshad", "OilFormationVolumeFactor", "RB_STB",
                 1.1178, 1.6229)
add_title("PETROSKY & FARSHAD CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 81 laboratory PVT analyses. Fluid samples were obtained from reservoirs located offshore Texas and Louisiana.")
add_db("Producing areas from Galveston Island, eastward, through Main Pass are represented.")
add_db("Many Gulf of Mexico crudes can be characterized as naphthenic or biodegraded oils.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.5781,  0.8519)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(45)",  "Quantity<Api>(16.3)")
add_synonym("yo", "api", "Api")
add_parameter("rs", "SCF_STB", "Solution GOR",  217,  1406)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  114,  288)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  1574,  6523)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Petrosky & Farshad")
add_ref("petrosky:1993")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaStanding", "OilFormationVolumeFactor", "RB_STB",
                 1.024, 2.15)
add_title("STANDING CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 105 experimentally determined bubble point pressures from 22 different Californian crude-oil-natural-gas mixtures.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.59,  0.95)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(63.8)",  "Quantity<Api>(16.5)")
add_parameter("rs", "SCF_STB", "Solution GOR",  20,  1425)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  100,  258)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb",  "psia", "Bubble point pressure")
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Standing")
add_ref("standing:1947")
add_ref("alShammasi:2001")
add_ref("mcCain:1990")
add_internal_note("The isothermal compressibility of crude oils (Co) used in this equation is obtained from the Vazquez & Beggs correlation.")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaTotalCFP", "OilFormationVolumeFactor", "RB_STB")
add_title("TOTAL COMPAGNIE FRANÇAISE DES PÉTROLES (CFP) CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 336 PVT analyses of crude oil from eastern Venezuela.")           
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("api",  "Api", "API oil gravity")
add_parameter("rs",  "SCF_STB", "Solution GOR")
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t",  "Fahrenheit", "Temperature")
add_parameter("p",  "psia", "Pressure")
add_parameter("pb",  "psia", "Bubble point pressure")
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Total CFP")
add_ref("totalCFP:1983")
add_ref("banzer:1996")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaVasquezBeggs", "OilFormationVolumeFactor", "RB_STB",
                  1.028, 2.226)
add_title("VAZQUEZ & BEGGS CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on more than 600 laboratory PVT analyses from fields all over the world.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.511,  1.35)
add_parameter("api",  "Api", "API oil gravity",  15.3,  59.3)
add_parameter("rs", "SCF_STB", "Solution GOR",  0,  2199)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  75,  294)
add_parameter("tsep", "Fahrenheit", "Separator temperature",  76,  150)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb",  "psia", "Bubble point pressure")
add_parameter("psep", "psia", "Separator pressure",  60,  565)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Vazquez & Beggs")
add_ref("vazquez:1980")
add_ref("alShammasi:2001")
add_ref("banzer:1996")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaCegarra", "OilFormationVolumeFactor", "RB_STB")
add_title("CEGARRA CORRELATION (STANDING CORRELATION APPLIED FOR CEGARRA), CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 375 consistent PVT analyses. The data bank was compiled from 8394 fields of eastern Venezuela subdivided into 5192 fields of light oil and 3202 fields of medium oil.")
add_db("The correlation takes the functional form of Standing correlation, which is based on 105 experimental data of 22 different crude-oil-natural-gas mixtures from California (USA).")
add_internal_note("The isothermal compressibility of crude oils (Co) used in this equation is obtained from the Vazquez & Beggs correlation.")
add_internal_note("The correlation's development ranges could not be verified because the original reference is not available. Date: August 22 2016.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.654,  1.337)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(41.8)",  "Quantity<Api>(22)")
add_synonym("yo", "api", "Api")
add_parameter("rs", "SCF_STB", "Solution GOR",  135,  4290)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  117,  354)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  735,  7303)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Cegarra")
add_ref("cegarra:2007")
add_ref("standing:1947")
add_ref("mcCain:1990")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaPerezML", "OilFormationVolumeFactor", "RB_STB",
                  1.032, 1.183)
add_title("PÉREZ, HENY & LAGO CORRELATION (EZEKWE-QUERIN-HUMPHREY CORRELATION USED BY PÉREZ, HENY & LAGO), CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_note("The correlation could not be verified because the original reference is not available. Date: August 22 2016.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.571,  0.981)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(12.9)",  "Quantity<Api>(6.4)")
add_synonym("yo", "api", "Api")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  38,  121)
add_parameter("t", "Fahrenheit", "Temperature",  112,  300)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  405,  1335)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Pérez, Heny & Lago")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 22 2016.")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaVelardeMcCain", "OilFormationVolumeFactor", "RB_STB",
                  1.040, 2.082)
add_title("VELARDE, BLASINGAME & McCAIN CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Based on 195 PVT reports, including 2097 data sets.")
add_parameter("yg", "Sgg", "Gas specific gravity",  0.561,  1.101)
add_parameter("yo", "Sg_do", "Oil specific gravity",  "Quantity<Api>(53.4)",  "Quantity<Api>(11.6)")
add_synonym("yo", "api", "Api")
add_parameter("rs",  "SCF_STB", "Solution GOR")
add_parameter("rsb", "SCF_STB", "Solution GOR at Pb",  102,  1808)
add_parameter("t", "Fahrenheit", "Temperature",  70,  307)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  106,  5312)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Velarde, Blasingame & McCain")
add_ref("velarde:1997")
add_ref("mcCain:1990")
add_internal_note("The solution gas-oil ratios in the database are total gas-oil ratios (obtained by adding the solution gas-oil ratios at the separator and the stock tank).")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaMillanArcia", "OilFormationVolumeFactor", "RB_STB",
                  1.014, 1.248)
add_title("MILLÁN-ARCIA CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Venezuelan heavy crudes were correlated.")
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("api",  "Api", "API oil gravity",  9,  20.2)
add_parameter("rs", "SCF_STB", "Solution GOR",  53,  459)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  87,  195)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  222.0,  3432.7)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Millán-Arcia")
add_ref("millan:1984")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 22 2016.")
end_correlation()

################################################################

## verificada con python
begin_correlation("BoaManucciRosales", "OilFormationVolumeFactor", "RB_STB",
                  1.050, 2.400)
add_title("MANUCCI & ROSALES CORRELATION, CALCULATION OF OIL FORMATION VOLUME FACTOR")
add_db("Crude oils of eastern Venezuela were correlated, by using differential separation data from laboratory PVT analysis.")
add_parameter("yg",  "Sgg", "Gas specific gravity")
add_parameter("api",  "Api", "API oil gravity",  14,  40)
add_parameter("rs", "SCF_STB", "Solution GOR",  180,  2500)
add_parameter("rsb",  "SCF_STB", "Solution GOR at Pb")
add_parameter("t", "Fahrenheit", "Temperature",  165,  304)
add_parameter("p",  "psia", "Pressure")
add_parameter("pb", "psia", "Bubble point pressure",  1230,  5035)
add_parameter("co",  "psia_1", "Coefficient of isothermal compressibility")
add_author("Manucci & Rosales")
add_ref("manucci:1968")
add_ref("perez:2001")
add_internal_note("The correlation could not be verified because the original reference is not available. Date: August 22 2016.")
end_correlation()