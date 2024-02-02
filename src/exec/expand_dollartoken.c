/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollartoken.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzaytoun <yzaytoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 12:35:59 by yzaytoun          #+#    #+#             */
/*   Updated: 2024/02/02 16:36:55 by yzaytoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_get_limiter(const char *string)
{
	int		i;

	i = 0;
	if (string == NULL)
		return (NULL);
	while (string[i] != '\0')
	{
		if (ft_isalnum(string[i]) == FALSE)
			return ((char *)string + i);
		i++;
	}
	return (NULL);
}

static char	*ft_get_stringvalue(
		char *variable_name, t_list *envlist, int laststatus)
{
	char	*value;
	char	*limiter;
	char	*stringprefix;

	value = NULL;
	limiter = ft_get_limiter(variable_name);
	stringprefix = ft_cutstr(variable_name, limiter);
	if (variable_name == NULL || variable_name[0] == '\0')
		return (NULL);
	else if (*variable_name == '?')
		value = ft_strjoin_get(ft_itoa(laststatus), variable_name + 1);
	else if (limiter != NULL)
		value = ft_strjoin_get(ft_getenv(stringprefix, envlist), limiter);
	else
		value = ft_getenv(variable_name, envlist);
	if (stringprefix != NULL)
		free(stringprefix);
	if (value == NULL)
		value = ft_strdup("");
	return (value);
}

static void	ft_expand_stringarray(
		char ***stringarray, int laststatus, t_list *envlist)
{
	int		count;
	char	*buffer;

	buffer = NULL;
	if (*stringarray == NULL || envlist == NULL)
		return ;
	count = 0;
	while ((*stringarray)[count] != NULL)
	{
		buffer = ft_get_stringvalue((*stringarray)[count], envlist, laststatus);
		if (buffer != NULL)
		{
			free((*stringarray)[count]);
			(*stringarray)[count] = buffer;
		}
		count++;
	}
}

static char	*ft_expand_dollarchain(
		const char *fullstring, int laststatus, t_list *envlist)
{
	char	*expandedstring;
	char	**stringarray;
	t_list	*list;

	expandedstring = NULL;
	stringarray = NULL;
	list = NULL;
	if (fullstring == NULL || envlist == NULL)
		return (NULL);
	list = ft_get_stringlist(fullstring);
	stringarray = ft_lstconvert_strarr(list, 0);
	ft_expand_stringarray(&stringarray, laststatus, envlist);
	expandedstring = ft_concat_strarray(stringarray, FALSE);
	ft_clear_strarray(stringarray);
	return (expandedstring);
}

char	*ft_expand_dollartoken(const char *argument, t_global *global)
{
	char	*value;
	int		dollarcount;

	value = NULL;
	global->expand_dollartoken = FALSE;
	if (argument == NULL || (global != NULL && global->envlist == NULL))
		return (NULL);
	dollarcount = ft_chrcount(argument, '$');
	if (dollarcount >= 1)
		value = ft_expand_dollarchain(argument, g_exit_status, global->envlist);
	else
		return ((char *)argument);
	if (value == NULL)
		value = ft_strdup("");
	system("leaks minishell");
	return (value);
}
