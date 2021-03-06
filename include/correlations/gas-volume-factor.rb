# coding: utf-8

begin_correlation("Bg", "GasVolumeFactor", "RCF_SCF")
add_title("CALCULATION OF GAS FORMATION VOLUME FACTOR")
add_parameter("t", "Rankine", "Temperature")
add_parameter("p", "psia", "Pressure")
add_parameter("z", "Zfactor", "Gas compressibility factor")
add_author("Standard Equation")
add_db("The calculation is based on the Real Gas Law.")
add_ref("takacs:2005")
add_ref("petroWiki:2016")
# add_ref("banzer:1996") Secondary reference
add_internal_note("The equation was verified by using secondary references: Bánzer (1996) and PetroWiki (2016). Date: September 28 2016.")
end_correlation()
