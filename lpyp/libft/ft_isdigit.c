#include "libft.h"

int	ft_isdigit(int c);

int	ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}
