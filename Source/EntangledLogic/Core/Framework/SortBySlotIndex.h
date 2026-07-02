#pragma once


// Used for sorting arrays by slot index
template <typename T>
static bool SortBySlotIndex(const T& A, const T& B)
{
	return A.SlotIndex < B.SlotIndex;
}