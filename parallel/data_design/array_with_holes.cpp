// https://bitsquid.blogspot.com/2011/09/managing-decoupling-part-4-id-lookup.html


// 但如果数组中有 50% 的对象和 50% 的空洞，循环就会占用两倍于所需内存。这似乎不太理想。 -> packed array

struct ID {
	unsigned index;
	unsigned inner_id;
};

struct Object {
	ID id;
	unsigned next;
};

struct System
{
	unsigned _next_inner_id;
	std::vector<Object> _objects;
	unsigned _freelist;

	System() {
		_next_inner_id = 0;
		_freelist = UINT_MAX;
	}

	inline bool has(ID id) {
		return _objects[id.index].id.inner_id == id.inner_id;
	}
	
	inline Object &lookup(ID id) {
		return _objects[id.index];
	}
	
	inline ID add() {
		ID id;
		id.inner_id = _next_inner_id++;
		if (_freelist == UINT_MAX) {
			Object o;
			id.index = _objects.size();
			o.id = id;
			_objects.push_back(o);
		} else {
			id.index = _freelist;
			_freelist = _objects[_freelist].next;
		}
		return id;
	}
	
	inline void remove(ID id) {
		Object &o = lookup(id);
		o.id.inner_id = UINT_MAX;
		o.next = _freelist;
		_freelist = id.index;
	}
};