#pragma once

struct SkipListNode;

typedef int slKeyType;
typedef int slValuetype;

#define SKIPLIST_MAX_LEVEL 32
#define SKIPLIST_LEVEL_P 6

#ifndef TRUE
#define TRUE 1
#define FALSE 0
typedef long BOOL;
#endif

typedef struct SkipListLevel{
	struct SkipListNode* forward;
} SkipListLevel;

typedef struct SkipListNode{
	slKeyType key;
	slValuetype value;
	SkipListLevel level[1];
}SkipListNode;

typedef SkipListNode* PSkipListNode;

class CSkipList
{
public:
	CSkipList(void);
	~CSkipList(void);

	PSkipListNode Set(slKeyType key, slValuetype value);
	PSkipListNode Find(slKeyType key);
	void Remove(slKeyType key);
	void RemoveAll();
	BOOL Get(slKeyType key, slValuetype& value);

	size_t GetItemCount();
	size_t GetLevel();
	size_t RandomLevel();

protected:
	PSkipListNode CreateNode(slKeyType key, slValuetype value, size_t level);

protected:
	PSkipListNode	m_pHeader;
	size_t	m_nLevel;
	size_t	m_nItemCount;
};

