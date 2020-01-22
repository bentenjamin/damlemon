/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwebb <bwebb@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/21 16:28:33 by bwebb             #+#    #+#             */
/*   Updated: 2020/01/22 17:14:11 by bwebb            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lemon.h"

void	initvisited(t_heart **heart)
{
	t_input	*input;
	t_vein	*vein;
	t_artery	*artery;

	artery = (*heart)->artery;
	resetvisits(heart);
	while (artery)
	{
		vein = artery->vein;
		while (vein)
		{
			input = (*heart)->input;
			while (input && (input->roomnode->id != vein->node->id))
				input = input->next;
			if (input && (input->roomnode->id != vein->node->id))
				input->roomnode->id = 1;
			vein = vein->next;
		}
		artery = artery->next;
	}
};



//write addveinids
void	addveinids(int id, t_veinids **veinids)
{
	t_veinids *curNode;
	t_veinids *newNode;

	curNode = *veinids;
	while (curNode && curNode->next)
		curNode = curNode->next;
	if (!(newNode = (t_veinids*)malloc(sizeof(t_veinids))))
		return (0);
	newNode->id = id;
	newNode->next = NULL;
	if (*veinids)
		curNode->next = newNode;
	else
		*veinids = newNode;
	// *((*veinids) ? veinids : &(curNode->next)) = newNode;
	return (1);
}

void	freeveinids(t_veinids **veinids)
{
	if ((*veinids)->next)
		freeveinids(&(*veinids)->next);
	free(*veinids);
}

int		search(t_heart **heart, t_vein **curvein)//fix to find 1 path at a time
{
	t_queue		*q;
	t_veinids	*veinids;
	int			i;

	pushq(&q, NULL, &(*heart)->network);
	while (q)
	{
		i = 0;
		while (q->node->links[i])
			if (!(q->node->links[i]->visited))
				pushq(&q, &q, &(q->node->links[i++]));
		if (!q->next || q->node->end)
			break ;
		q = q->next;
	}
	if (!q->node->end)
		return (0);
	while (q->parent)
	{
		addveinids(q->node->id, &veinids);
		q = q->parent;
	}
	freeq(q, veinids, curvein);
	freeveinids(&veinids);
	return (1);
}

int		bfs(t_heart **heart)
{
	int	i;
	t_artery	*artery;
	
	i = 0;
	while (1)
	{
		artery = addarteryNode(&(*heart)->artery, ++i);
		initvisited(heart);
		if (!search(heart, &(artery->vein)))
			break ;
		artery->ants = veinlen(artery->vein);
	}
	popart((*heart)->artery);
	return((*heart)->artery ? 1 : 0);
}