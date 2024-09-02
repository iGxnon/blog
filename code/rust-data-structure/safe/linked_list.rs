struct List {
    head: Link,
}

type Link = Option<Box<Node>>;

struct Node {
    elem: i32,
    next: Link,
}

impl List {
    fn new() -> Self {
        Self {
            head: None,
        }
    }

    fn push_front(&mut self, elem: i32) {
        let node = Box::new(Node {
            elem,
            next: std::mem::replace(&mut self.head, None),
        });

        self.head = Some(node);
    }

    fn pop_front(&mut self) -> Option<i32> {
        match std::mem::replace(&mut self.head, None) {
            None => None,
            Some(head) => {
                self.head = head.next;
                Some(head.elem)
            }
        }
    }
}

fn main() {
    let mut list = List::new();
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    list.push_front(4);
    assert_eq!(list.pop_front(), Some(4));
    while let Some(front) = list.pop_front() {
        print!("{}, ", front);
    }
    println!();
}