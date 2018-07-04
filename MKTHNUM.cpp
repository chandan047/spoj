// https://www.spoj.com/problems/MKTHNUM/

#define _CRT_SECURE_NO_WARNINGS

#include <bits/stdc++.h>

using namespace std;

template<typename IT> void merge(IT begin, IT middle, IT end, IT res)
{
	IT a = begin, b = middle, r = res;

	while (a < middle && b < end)
		if (*a < *b) *r++ = *a++;
		else *r++ = *b++;

		while (a < middle) *r++ = *a++;
		while (b < end) *r++ = *b++;
		while (begin < end) *begin++ = *res++;
}

template<typename IT> void mergesort(IT begin, IT end, IT res)
{
	int s = end - begin;
	if (s > 1)
	{
		IT middle = begin + s / 2;
		mergesort(begin, middle, res);
		mergesort(middle, end, res);
		merge(begin, middle, end, res);
	}
}

#define MAX 100000

// Persistent segment tree
struct PST {
	int freq;
	PST* left;
	PST* right;

	PST() {
		left = nullptr;
		right = nullptr;
		freq = 0;
	}
};

int arr[MAX], arr_s[MAX], freq[MAX];

int searchunique(int l, int r, int val) {
	if (l == r)
		return l;

	int mid = l + (r - l) / 2;
	if (val <= arr_s[mid])
		return searchunique(l, mid, val);
	else
		return searchunique(mid + 1, r, val);
}

// keep n roots for n persistent segment trees
PST* root[MAX + 1];

int build(PST* node, int l, int r) {
	if (l == r) {
		node->freq = freq[l];
		return freq[l];
	}

	int f = 0, mid = l + (r - l) / 2;
	
	node->left = new PST();
	f += build(node->left, l, mid);

	node->right = new PST();
	f += build(node->right, mid + 1, r);

	node->freq = f;

	return f;
}

void update(PST* prev, PST* curr, int idx, int l, int r) {
	curr->freq = prev->freq + 1;

	if (l == r) {
		freq[l]++;
		return;
	}

	int mid = l + (r - l) / 2;

	if (idx <= mid) {
		curr->left = new PST;
		curr->right = prev->right;
		update(prev->left, curr->left, idx, l, mid);
	}
	else {
		curr->left = prev->left;
		curr->right = new PST;
		update(prev->right, curr->right, idx, mid + 1, r);
	}
}

int query(PST* a, PST *b, int k, int l, int r) {
	if (l == r)
		return l;

	int mid = l + (r - l) / 2;
	int count = b->left->freq - a->left->freq;
	if (count >= k)
		return query(a->left, b->left, k, l, mid);
	else
		return query(a->right, b->right, k - count, mid + 1, r);
}

int main() {
	freopen("input.txt", "r", stdin);

	int i, j;

	int n, m;
	scanf("%d %d", &n, &m);

	for (i = 0; i < n; i++) {
		scanf("%d", arr + i);
		freq[i] = arr[i];
	}

	mergesort(freq, freq + n, arr_s);

	for (i = 0; i < n; i++)
		freq[i] = 0;

	// get frequencies of unique
	i = 1, j = 0;
	while (i < n) {
		if (arr_s[i] != arr_s[j]) j++;
		i++;
	}
	j++;

	for (i = 0; i < n + 1; i++)
		root[i] = new PST();

	int idx;

	build(root[0], 0, j - 1);

	for (i = 0; i < n; i++) {
		idx = searchunique(0, j - 1, arr[i]);
		update(root[i], root[i + 1], idx, 0, j - 1);
	}

	int a, b, k;
	for (i = 0; i < m; i++) {
		scanf("%d %d %d", &a, &b, &k);
		idx = query(root[a - 1], root[b], k, 0, j - 1);
		printf("%d\n", arr_s[idx]);
	}

	return 0;
}