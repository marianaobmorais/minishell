#include "../../includes/minishell.h"

/**
 * @brief Checks for overflow or underflow during numeric conversion.
 *
 * Determines if adding a digit to the current result `res` would cause an
 * overflow or underflow based on the provided sign. Returns `LLONG_MAX` or
 * `LLONG_MIN` in such cases, otherwise returns the current result.
 *
 * @param digit The digit to be added to the result.
 * @param sign The sign of the number (-1 or 1).
 * @param res The current numeric result.
 * @return The updated result, or `LLONG_MAX`/`LLONG_MIN` on overflow/underflow.
 */
static long long	ft_check_flow(int digit, long long sign, long long res)
{
	if (sign == 1 && (res > LLONG_MAX / 10
			|| (res == LLONG_MAX / 10 && digit > LLONG_MAX % 10)))
		return (LLONG_MAX);
	if (sign == -1 && (res > LLONG_MAX / 10
			|| (res == LLONG_MAX / 10 && digit > (LLONG_MAX % 10) + 1)))
		return (LLONG_MIN);
	return (res);
}

/**
 * @brief Converts a string to a long long integer with overflow handling.
 *
 * Parses a numeric string and returns its value as a `long long`. Handles
 * optional '+' or '-' sign and leading whitespace. If overflow or underflow
 * occurs, it returns `LLONG_MAX` or `LLONG_MIN` respectively. Sets a boolean
 * flag `is_sign` to indicate whether the input string contains a sign.
 *
 * @param nptr The string to convert.
 * @param is_sign Pointer to a boolean to indicate if a sign was detected.
 * @return The converted number, or `LLONG_MAX`/`LLONG_MIN` on
 *         overflow/underflow.
 */
long long	ft_atoll(const char *nptr, bool *is_sign)
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
		*is_sign = true;
		if (nptr[i] == '-')
			sign = -1;
		++i;
	}
	while (nptr[i] != '\0' && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		res = ft_check_flow(nptr[i] - '0', sign, res);
		if (res == LLONG_MAX || res == LLONG_MIN)
			return (res);
		res = res * 10 + nptr[i] - '0';
		++i;
	}
	return (res * sign);
}
