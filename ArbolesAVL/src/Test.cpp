#include "Avltree.h"

int main() {
	Avltree t;
	int ch = 10;
	//clrscr();
	cout << "\n\nProbando la clase AVLTREE\n\n";

	while (ch != 20) {
		cout
				<< "\n1.Insertar Item\n2.Inorder Transversal\n3.Preorder Transversal\n4.Postorder Transversal\n5.Buscar\n6.Obtener padre\n7.Altura\n8.Balancear\n9.Borrar\n10.Salir";
		cout << "\nIngrese seleccion: ";
		cin >> ch;
		switch (ch) {
		case 1: {
			int i;
			cout << "\nIngrese item: ";
			cin >> i;
			t.insert(i);
			break;
		}
		case 2: {
			cout << "\nInorder Transversal...\n";
			t.inorder(t.root);
			break;
		}
		case 3: {
			cout << "\nPreorder Transversal...\n";
			t.preorder(t.root);
			break;
		}
		case 4: {
			cout << "\nPostorder Transversal...\n";
			t.postorder(t.root);
			break;
		}
		case 5: {
			int i;
			cout << "\nInrese elemento a ser buscado: ";
			cin >> i;
			if (t.getnode(i) != NULL)
				cout << "\nElemento encontrado.";
			else
				cout << "\nElemento no encontrado.";
			break;
		}
		case 6: {
			int i;
			cout << "\nIngrese elemento: ";
			cin >> i;
			Node *t1 = t.getparent(i);
			if (t1 != NULL)
				cout << "\nPadre en " << t1->data;
			else
				cout << "\nElemento no encontrado o es el nodo padre";
			break;
		}
		case 7: {
			cout << "\nAltura del arbol:" << t.height(t.root);
			break;
		}
		case 8: {
			int i;
			cout << "\nIngrese el elemento :";
			cin >> i;
			cout << "\nBalancear: " << t.balance(t.getnode(i));
			break;
		}
		case 9: {
			int i;
			cout << "\nIngrese el elemento a ser borrado: ";
			cin >> i;
			t.remove(i);
			break;
		}
		default:
			ch = 20;
		}
	}

	return 0;
}
