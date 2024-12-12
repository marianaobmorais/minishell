#include "../../includes/minishell.h"

/**
 * @brief Checks for integer overflow or underflow during conversion.
 *
 * This function verifies if adding a digit to the result would cause the value
 * to exceed the range of a signed `long long` integer. If overflow or underflow
 * is detected, it returns the maximum or minimum value of `long long`
 * respectively. Ensures safe accumulation of digits during numeric conversion.
 *
 * @param digit The current digit to be added to the result.
 * @param sign The sign of the number being processed (`1` for positive, `-1`
 *        for negative).
 * @param res The current result before adding the digit.
 * @return The adjusted result if no overflow/underflow occurs, or
 *         `LLONG_MAX`/`LLONG_MIN` otherwise.
 */
static long long	ft_check_flow(int digit, long long sign, long long res)
{
	if (res > LLONG_MAX / 10
		|| (res == LLONG_MAX / 10 && digit > LLONG_MAX % 10))
	{
		if (sign == 1)
			return (LLONG_MAX);
		else
			return (LLONG_MIN);
	}
	return (res);
}

/**
 * @brief Converts a string to a `long long` integer.
 *
 * This function parses a null-terminated string and converts it to a signed
 * `long long` integer. It skips leading whitespace, handles an optional sign,
 * and processes numeric digits.
 *
 * @param nptr The input string to be converted.
 * @return The converted `long long` integer, or `LLONG_MAX`/`LLONG_MIN` on
 *         overflow/underflow.
 */
long long	ft_atoll(const char *nptr)
{
	int			i;
	long long	sign;
	long long	res;

	i = 0;
	sign = 1;
	res = 0;
	while ((nptr[i] == 32) || (nptr[i] > 8 && nptr[i] < 14))
		++i;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		++i;
	}
	while (nptr[i] != '\0' && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		res = ft_check_flow(nptr[i] - '0', sign, res);
		if (res > LLONG_MAX || res < LLONG_MIN)
			return (res);
		res = res * 10 + nptr[i] - '0';
		++i;
	}
	return (res * sign);
}
