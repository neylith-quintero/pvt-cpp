# coding: utf-8

begin_correlation("ZFactorSarem", "GasCompressibility", "ZFactor")
add_title("SAREM FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine") # TODO falta cotas
add_parameter("psc", "psia")
add_ref("sarem:1961")
add_ref("standing:1942")
add_ref("banzer:1996")
add_db("The correlation was obtained by using Legendre polynomials of up to five degree to fit the Standing-Katz curves for the gas compressibility factor (Z).")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 22 2016.")
end_correlation()

################################################################

begin_correlation("ZFactorHallYarborough", "GasCompressibility", "ZFactor")
add_title("HALL & YARBOROUGH FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine") # TODO falta cotas
add_parameter("psc", "psia")
add_ref("hall:1973")
add_ref("yarborough:1974")
add_ref("standing:1942")
add_ref("banzer:1996")
add_db("Based on the equation of state of Starling-Carnahan for the calculation of Z-factor. The coefficients of the equation were obtained by fitting the Standing-Katz curves.")
add_db("The equation is solved by using the Newton-Raphson iteration method.")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 23 2016.")
add_internal_note("The application ranges and data bank information was obtained from Takacs (1989).")
end_correlation()

################################################################

begin_correlation("ZFactorDranchukPR", "GasCompressibility", "ZFactor")
add_title("DRANCHUK ET AL. FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine") # TODO falta cotas
add_parameter("psc", "psia")
add_ref("dranchuk:1973")
add_ref("standing:1942")
add_ref("takacs:1989")
add_ref("banzer:1996")
add_db("This method is a result of fitting the Benedict-Webb-Rubin (BWR) equation of state for the Z-factor. 1500 data points from the original Standing-Katz chart were used.")
add_db("The equation is solved by using the Newton-Raphson iteration method.")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 23 2016.")
add_internal_note("The application ranges and data bank information was obtained from Takacs (1989).")
end_correlation()

################################################################

begin_correlation("ZFactorDranchukAK", "GasCompressibility", "ZFactor")
add_title("DRANCHUK & ABOU-KASSEM FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine") # TODO falta cotas
add_parameter("psc", "psia")
add_ref("dranchuk:1975")
add_ref("standing:1942")
add_ref("takacs:1989")
add_ref("banzer:1996")
add_db("This method is a result of fitting the Starling-Carnahan equation of state for the Z-factor. 1500 data points from the original Standing-Katz chart were used.")
add_db("The equation is solved by using the Newton-Raphson iteration method.")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 23 2016.")
add_internal_note("The application ranges and data bank information was obtained from Takacs (1989).")
add_internal_note("The lower limit for Ppr (when Tpr's range is from 0.7 to 1.0) was taken from the development ranges of the correlation presented by Standing & Katz (1942).")
end_correlation()

################################################################

begin_correlation("ZFactorGopal", "GasCompressibility", "ZFactor")
add_title("GOPAL FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine") # TODO falta cotas
add_parameter("psc", "psia")
add_ref("gopal:1977")
add_ref("standing:1942")
add_ref("banzer:1996")
add_ref("takacs:1989")
add_db("Gopal presented a non-iterative method for calculating the Z-factor. The coefficients of the equations were obtained by fitting the Standing-Katz curves.")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 23 2016.")
add_internal_note("The application ranges and data bank information was obtained from Takacs (1989).")
end_correlation()

################################################################

begin_correlation("ZFactorBrillBeggs", "GasCompressibility", "ZFactor")
add_title("BRILL & BEGGS FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine") # TODO falta cotas
add_parameter("psc", "psia")
add_ref("brill:1974")
add_ref("standing:1942")
add_ref("banzer:1996")
add_db("The equation was obtained by applying adjustment methods to the Standing-Katz curves for the gas compressibility factor (Z).")
add_internal_note("The original reference is not available. The correlation was verified by using a secondary reference: Bánzer (1996). Date: September 26 2016.")
end_correlation()

################################################################

begin_correlation("ZFactorPapay", "GasCompressibility", "ZFactor")
add_title("PAPAY FIT METHOD, CALCULATION OF THE GAS COMPRESSIBILITY FACTOR")
add_parameter("t", "Rankine")
add_parameter("p", "psia")
add_parameter("tsc", "Rankine") # TODO falta cotas
add_parameter("psc", "psia")
add_ref("papay:1968")
add_ref("standing:1942")
add_ref("takacs:1989")
add_ref("banzer:1996")
add_internal_note("The original reference is not available. The correlation was verified by using secondary references: Bánzer (1996) and Takacs (1989). Date: September 23 2016.")
add_internal_note("The data bank information was obtained from Takacs (1989).")
add_internal_note("The application ranges were presented by Bánzer (1996).")
end_correlation()
