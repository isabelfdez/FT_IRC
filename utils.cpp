/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 16:22:07 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/21 18:50:36 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool	find_command(std::string command, std::list<std::string> commands)
{
	for (std::list<std::string>::iterator it = commands.begin(); it != commands.end(); ++it)
		if (*it == command) // Esto no se si esta bien
			return (true);
	return (false);
}



void	send_reply(std::string replay, std::string str, int fd)
{
	std::string message;

	message.assign(":ft_irc.com ");
	message.append(replay);
	message.append(" * ");
	message.append(str);
	message.append("\n");
	send(fd, message.c_str(), message.length(), 0);

}

void	send_error(std::string error, std::string str, int fd)
{
	std::string message;

	message.assign(":ft_irc.com ");
	message.append(error);
	message.append(" * ");
	message.append(str);
	message.append("\n");
    send(fd, message.c_str(), message.length(), 0);
}

void	send_message(std::string & message, int & fd, User * usr)
{
	std::string mask;

	//mask.append(":");
	//mask.append(usr->getNick());
	//mask.append("!");
	//mask.append(usr->getUserName());
	//mask.append("@ft_irc.com");
	mask.append(usr->getMask());
	mask.append(" ");
	mask.append(message);
	mask.append("\n");
	send(fd, mask.c_str(), mask.length(), 0);
}

void	send_message_channel(std::string & message, User * usr, Channel * chnl)
{
	std::string mask;

	mask.append(usr->getMask());
	//mask.append(":");
	//mask.append(usr->getNick());
	//mask.append("!");
	//mask.append(usr->getUserName());
	//mask.append("@ft_irc.com");
	mask.append(" ");
	mask.append(message);
	mask.append("\n");
	chnl->sendMsgChannel(mask);
}

void complete_registration(User * usr)
{
	std::string mask;
	std::string	s;

	// Cambiamos el valor del estado de registro
	usr->setRegistered(true);
	// Escribimos la mask
	mask.append(":");
	mask.append(usr->getNick());
	mask.append("!");
	mask.append(usr->getUserName());
	mask.append("@ft_irc.com");
	usr->setMask(mask);
	s.append("Welcome to the Internet Relay Network\n");
	mask.erase(0,1);
	s.append(mask);
	send_reply(RPL_WELCOME, s, usr->getsockfd());
}

static int	ft_countwords(char const *s, char c)
{
	int		i;
	int		counter;

	counter = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			i++;
			continue ;
		}
		counter++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (counter);
}

static int	ft_size(char const *s, char c, int j)
{
	int		counter;

	counter = 0;
	while (s[j] == c)
		j++;
	while (s[j] && s[j] != c)
	{
		counter++;
		j++;
	}
	return (counter + 1);
}

static int	ft_cpyword(char const *s, char c, int j, char *str)
{
	int		i;

	i = 0;
	while (s[j] == c)
		j++;
	while (s[j] && s[j] != c)
		str[i++] = s[j++];
	str[i] = 0;
	return (j);
}

char		**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	int		words;
	char	**tab;

	j = 0;
	if (!s)
		return (NULL);
	i = ft_countwords(s, c);
	if (!(tab = (char **)malloc(sizeof(char *) * (i + 1))))
		return (NULL);
	tab[i] = NULL;
	words = i;
	i = 0;
	while (i < words)
	{
		tab[i] = (char *)malloc(sizeof(char) * ft_size(s, c, j));
		j = ft_cpyword(s, c, j, tab[i]);
		i++;
	}
	return (tab);
}

int		ft_isalpha(int c)
{
	if (65 <= c && c <= 90)
		return (1);
	else if (97 <= c && c <= 122)
		return (1);
	else
		return (0);
}

int		ft_isalnum(int c)
{
	if (65 <= c && c <= 90)
		return (1);
	else if (97 <= c && c <= 122)
		return (1);
	else if (48 <= c && c <= 57)
		return (1);
	else
		return (0);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

bool	ft_isspecial(char c)
{
	if (c != '[' && c != ']'
		&&  c != '\\' &&  c != '`' && c != '_'
		&& c != '^' && c != '{' && c !='|' && c != '}')
		return (false);
	return (true);
}

char	*ft_substr(char const *s, char c)
{
	size_t	i;
	size_t	len;
	char	*str;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] && s[i] != c)
		i++;
	len = i;
	i = 0;
	if (!(str = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = 0;
	return (str);
}
