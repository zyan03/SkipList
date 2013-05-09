#include "StdAfx.h"
#define _CRT_RAND_S
#include <stdlib.h>
#include <windows.h>
#include <new>
#include "SkipList.h"


CSkipList::CSkipList(void)
{
	m_pHeader = CreateNode(0, 0, SKIPLIST_MAX_LEVEL);
	m_nItemCount = 0;
	m_nLevel = 0;
}


CSkipList::~CSkipList(void)
{
	RemoveAll();
	delete m_pHeader;
	m_pHeader = NULL;
}


size_t CSkipList::RandomLevel()
{
	size_t nLevel = 0;
	UINT nRandomNum = 0;

	while(nLevel < SKIPLIST_MAX_LEVEL)
	{
		if (rand_s(&nRandomNum) != 0)
			break;

		if ((nRandomNum % 10) < SKIPLIST_LEVEL_P)
			break;

		nLevel++;
	}

	return nLevel;
}

PSkipListNode CSkipList::CreateNode( slKeyType key, slValuetype value, size_t level )
{
	size_t nodeSize = sizeof(SkipListNode) + level * sizeof(SkipListLevel);
	void *ptr = malloc(nodeSize);
	memset(ptr, 0, nodeSize);
	
	PSkipListNode pNewNode = new(ptr) SkipListNode;

	pNewNode->key = key;
	pNewNode->value = value;

	m_nItemCount++;

	return pNewNode;
}

void CSkipList::RemoveAll()
{
	PSkipListNode pNode = m_pHeader->level[0].forward;
	PSkipListNode pNodeForDel = NULL;
	while(pNode)
	{
		pNodeForDel = pNode;
		pNode = pNode->level[0].forward;
		delete pNodeForDel;
	}
	m_nItemCount = 0;
}

PSkipListNode CSkipList::Set( slKeyType key, slValuetype value )
{
	int i = 0;
	size_t level = 0;
	PSkipListNode pNode = m_pHeader;
	PSkipListNode pNewNode = NULL;
	PSkipListNode update[SKIPLIST_MAX_LEVEL];
	
	memset(update, 0, sizeof(PSkipListNode) * SKIPLIST_MAX_LEVEL);

	for (i = m_nLevel; i >= 0; i--)
	{
		while(pNode->level[i].forward && (pNode->level[i].forward->key < key))
		{
			pNode = pNode->level[i].forward;
		}
		update[i] = pNode;
	}

	if (pNode->level[0].forward && (pNode->level[0].forward->key == key))
	{
		pNode->level[0].forward->value = value;
		return pNode;
	}

	level = RandomLevel();

	pNewNode = CreateNode(key, value, level);

	if (level > m_nLevel)
	{
		for (i = level; i > (int)m_nLevel; i--)
		{
			update[i] = m_pHeader;
		}
		m_nLevel = level;
	}

	for (i = 0; i <= (int)level; i++)
	{
		pNewNode->level[i].forward = update[i]->level[i].forward;
		update[i]->level[i].forward = pNewNode;
	}

	return pNewNode;
}

size_t CSkipList::GetItemCount()
{
	return m_nItemCount;
}

size_t CSkipList::GetLevel()
{
	return m_nLevel;
}

PSkipListNode CSkipList::Find( slKeyType key )
{
	PSkipListNode pNode = m_pHeader;
	int i = 0;
	for (i = m_nLevel; i >= 0; i--)
	{
		while(pNode->level[i].forward && (pNode->level[i].forward->key < key))
		{
			pNode = pNode->level[i].forward;
		}

		if (pNode->level[i].forward && pNode->level[i].forward->key == key)
		{
			return pNode;
		}
	}

	return NULL;
}

void CSkipList::Remove( slKeyType key )
{
	PSkipListNode pNode = m_pHeader;
	int i = 0;
	PSkipListNode update[SKIPLIST_MAX_LEVEL];
	PSkipListNode pNodeForDel = NULL;

	memset(update, 0, sizeof(PSkipListNode) * SKIPLIST_MAX_LEVEL);

	for (i = m_nLevel; i >= 0; i--)
	{
		while(pNode->level[i].forward && (pNode->level[i].forward->key < key))
		{
			pNode = pNode->level[i].forward;
		}

		update[i] = pNode;
	}

	if (pNode->level[0].forward && (pNode->level[0].forward->key == key))
	{
		pNodeForDel = pNode->level[0].forward;
		for (i = m_nLevel; i >= 0; i--)
		{
			if (update[i]->level[i].forward == pNodeForDel)
			{
				update[i]->level[i].forward = pNodeForDel->level[i].forward;
			}
		}
		delete pNodeForDel;

		//update level if the remove node,change the level
		while(m_nLevel > 0 && m_pHeader->level[m_nLevel].forward == NULL)
			m_nLevel--;

		m_nItemCount--;
	}
}

BOOL CSkipList::Get( slKeyType key, slValuetype& value )
{
	PSkipListNode pFind = Find(key);
	if (pFind != NULL)
	{
		value = pFind->value;
		return TRUE;
	}

	return FALSE;
}
