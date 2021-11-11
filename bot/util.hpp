/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 18:18:20 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/11 18:19:51 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _UTIL_HPP
# define _UTIL_HPP

# include <vector>
# include <string>

std::vector<std::string> parser( std::string buffer );
std::vector<std::string> split( std::string buffer, char c );


#endif