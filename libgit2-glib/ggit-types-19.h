/*
 * ggit-types-19.h
 * This file is part of libgit2-glib
 *
 * Copyright (C) 2025 - Alberto Fanjul
 *
 * libgit2-glib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * libgit2-glib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libgit2-glib. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GGIT_TYPES_19_H__
#define __GGIT_TYPES_19_H__

/**
 * GgitConfigLevel:
 * @GGIT_CONFIG_LEVEL_PROGRAMDATA: System-wide on Windows, for compatibility with portable git.
 * @GGIT_CONFIG_LEVEL_SYSTEM: System-wide configuration file.
 * @GGIT_CONFIG_LEVEL_XDG: XDG compatible configuration file (.config/git/config).
 * @GGIT_CONFIG_LEVEL_GLOBAL: User-specific configuration file, also called Global configuration file.
 * @GGIT_CONFIG_LEVEL_LOCAL: Repository specific configuration file.
 * @GIT_CONFIG_LEVEL_WORKTREE: Worktree specific configuration file; $GIT_DIR/config.worktree
 * @GGIT_CONFIG_LEVEL_APP: Application specific configuration file; freely defined by applications.
 * @GGIT_CONFIG_LEVEL_HIGHEST: Represents the highest level of a config file.
 *
 * Priority level of a config file.
 * These priority levels correspond to the natural escalation logic
 * (from higher to lower) when searching for config entries in git.git.
 */
typedef enum
{
	GGIT_CONFIG_LEVEL_PROGRAMDATA = 1,
	GGIT_CONFIG_LEVEL_SYSTEM      = 2,
	GGIT_CONFIG_LEVEL_XDG         = 3,
	GGIT_CONFIG_LEVEL_GLOBAL      = 4,
 	GGIT_CONFIG_LEVEL_LOCAL       = 5,
	GGIT_CONFIG_LEVEL_WORKTREE    = 6,
	GGIT_CONFIG_LEVEL_APP         = 7,
	GGIT_CONFIG_LEVEL_HIGHEST     = -1
} GgitConfigLevel;

typedef enum
{
	GGIT_CHECKOUT_SAFE                    = 0,
	GGIT_CHECKOUT_FORCE                   = (1u << 1),
	GGIT_CHECKOUT_RECREATE_MISSING        = (1u << 2),
	GGIT_CHECKOUT_ALLOW_CONFLICTS         = (1u << 4),
	GGIT_CHECKOUT_REMOVE_UNTRACKED        = (1u << 5),
	GGIT_CHECKOUT_REMOVE_IGNORED          = (1u << 6),
	GGIT_CHECKOUT_UPDATE_ONLY             = (1u << 7),
	GGIT_CHECKOUT_DONT_UPDATE_INDEX       = (1u << 8),
	GGIT_CHECKOUT_NO_REFRESH              = (1u << 9),
	GGIT_CHECKOUT_SKIP_UNMERGED           = (1u << 10),
	GGIT_CHECKOUT_USE_OURS                = (1u << 11),
	GGIT_CHECKOUT_USE_THEIRS              = (1u << 12),
	GGIT_CHECKOUT_DISABLE_PATHSPEC_MATCH  = (1u << 13),
	GGIT_CHECKOUT_SKIP_LOCKED_DIRECTORIES = (1u << 18),
	GGIT_CHECKOUT_DONT_OVERWRITE_IGNORED  = (1u << 19),
	GGIT_CHECKOUT_CONFLICT_STYLE_MERGE    = (1u << 20),
	GGIT_CHECKOUT_CONFLICT_STYLE_DIFF3    = (1u << 21),
	GGIT_CHECKOUT_DONT_REMOVE_EXISTING    = (1u << 22),
	GGIT_CHECKOUT_DONT_WRITE_INDEX        = (1u << 23),
	GGIT_CHECKOUT_DRY_RUN                 = (1u << 24),
	GGIT_CHECKOUT_CONFLICT_STYLE_ZDIFF3   = (1u << 25),
	GGIT_CHECKOUT_NONE                    = (1u << 30),
	GGIT_CHECKOUT_UPDATE_SUBMODULES       = (1u << 16),
	GGIT_CHECKOUT_UPDATE_SUBMODULES_IF_CHANGED = (1u << 17)
} GgitCheckoutStrategy;

#endif /* __GGIT_TYPES_19_H__ */
