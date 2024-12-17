/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 18:25:36 by mariaoli          #+#    #+#             */
/*   Updated: 2024/12/16 18:25:37 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Frees the resources associated with an EXEC node.
 *
 * This function frees the memory allocated for the arguments of an EXEC node. 
 * It checks if the `args` list is non-NULL and, if so, it deallocates the list
 * of arguments associated with the EXEC node. The EXEC node itself is not
 * freed here, as it's handled elsewhere.
 *
 * @param exec_node A pointer to the EXEC node whose arguments should be freed.
 */
static void	ft_free_exec(t_exec *exec_node)
{
	if (exec_node->args)
	{
		ft_free_list(*exec_node->args);
		free(exec_node->args);
	}
}

/**
 * @brief Frees the resources associated with a redirection node.
 *
 * This function frees the memory allocated for the target of a redirection
 * node. It checks if the `target` list is non-NULL and, if so, deallocates the
 * list of one target token associated with the redirection node. The
 * redirection node itself is not freed here, as it's handled elsewhere.
 *
 * @param redir_node A pointer to the redirection node whose target should be
 *        freed.
 */
static void	ft_free_redir(t_redir *redir_node)
{
	if (redir_node->target)
	{
		ft_free_list(*redir_node->target);
		free(redir_node->target);
	}
}

/**
 * @brief Recursively frees a tree of nodes, deallocating memory for each node
 *        and its associated data.
 *
 * This function takes a node of type `t_node` (or its subtypes like `t_exec`,
 * `t_redir`) and frees all associated resources, including left and right
 * subtrees, as well as specific data contained within the node. It handles
 * different node types such as ROOT, AND, OR, PIPE, SUB_ROOT, EXEC, and REDIR,
 * ensuring all relevant structures (e.g., argument lists for EXEC nodes,
 * target lists for REDIR nodes) are properly freed.
 *
 * - For nodes of type `ROOT`, `AND`, `OR`, `PIPE`, or `SUB_ROOT`, it
 *   recursively frees the left and right subtrees.
 * - For nodes of type `EXEC`, `EXPORT`, or `EXPORT_AP`, it frees the argument
 *   list (`args`).
 * - For redirection nodes (`OUTFILE`, `INFILE`, `APPEND`, `HEREDOC`), it frees
 *   the target list (`target`) and any subsequent tokens (`next`).
 * - After processing the node's contents, it frees the node itself.
 *
 * @param root A pointer to the node to be freed. This can be any node in the
 *        tree, including root nodes, logical operators, pipe nodes, or
 *        subtrees.
 */
static void	ft_free_node(void *root)
{
	t_node	*node;

	if (!root)
		return ;
	node = (t_node *)root;
	if (node->type == PIPE)
	{
		if (node->left)
			ft_free_node(node->left);
		if (node->right)
			ft_free_node(node->right);
	}
	else if (node->type == OUTFILE || node->type == INFILE
		|| node->type == APPEND || node->type == HEREDOC)
	{
		ft_free_redir((t_redir *)root);
		if (((t_redir *)root)->next)
			ft_free_tree(((t_redir *)root)->next);
	}
	else if (node->type == EXEC || node->type == EXPORT
		|| node->type == EXPORT_AP)
		ft_free_exec((t_exec *)root);
	free(node);
}

/**
 * @brief Frees the entire tree structure, starting from the root node.
 *
 * This function acts as the entry point to deallocate the entire tree of
 * nodes, starting with the given root node. It calls the `ft_free_node`
 * function to recursively free all nodes in the tree, including all associated
 * resources such as subtrees and data within the nodes. The function ensures
 * that memory is properly freed for the entire tree structure, including EXEC
 * nodes, REDIR nodes, and logical operator nodes (ROOT, AND, OR, PIPE, etc.).
 *
 * @param root A pointer to the root node of the tree structure. The tree and
 *        all of its nodes, including any subtrees, will be recursively freed
 *        starting from this node.
 */
void	ft_free_tree(void *root)
{
	if (!root)
		return ;
	ft_free_node(root);
}
