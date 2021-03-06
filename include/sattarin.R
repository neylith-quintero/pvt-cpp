
sattarin <- function (API, t)
{
    if (API < 28)
    {
        a <- -5.8936e7 * (t ^ 2) + 3.511e10 * t - 5.2145e12
        b <- 0.00418 * (t ^ 2) - 2.50406 * t + 368.78706
        a * (API ^ b)
    }
    else
    {
        a <- (0.00735 * (t ^ 2)) - 4.3175*t + 641.3572
        b <- -1.51*t + 568.84
        d <- exp(b / API)        
        a * d / API
    }
}
