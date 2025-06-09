/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnemmass <hnemmass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 17:16:39 by yhajbi            #+#    #+#             */
/*   Updated: 2025/06/08 19:31:38 by hnemmass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_h

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <curses.h>
# include <term.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/types.h>
# include <dirent.h>

# include "structs.h"
# include "tokenizer.h"
# include "parsing.h"
# include "environment.h"
# include "utils.h"
# include "get_next_line.h"
# include "execution.h"

# define PROMPT	"minishell> "


#endif
