/*
 * Computação Concorrente
 * Laboratório 6, atividade 5
 * Aluno: Gabriel da Fonseca Ottoboni Pinho
 * DRE: 119043838
 */

public class Atv5 {
	private static class IncrementThread extends Thread {
		private final int startIndex;

		public IncrementThread(int startIndex) {
			this.startIndex = startIndex;
		}

		public void run() {
			for (int i = startIndex; i < array.length; i += numThreads) {
				array[i]++;
			}
		}
	}

	private static void die(String message) {
		System.out.println(message);
		System.exit(1);
	}

	private static int numThreads;
	private static int[] array;

	public static void main(String[] args) {
		Thread[] threads;
		int arraySize = 0;
		boolean correct = true;

		if (args.length != 2) {
			die("Uso: executável TAMANHO_ARRAY NÚMERO_THREADS");
		}

		try {
			arraySize = Integer.parseInt(args[0]);
			numThreads = Integer.parseInt(args[1]);
		} catch (NumberFormatException e) {
			die("TAMANHO_ARRAY ou NÚMERO_THREADS inválido.");
		}

		if (arraySize < 1 || numThreads < 1) {
			die("TAMANHO_ARRAY e NÚMERO_THREADS devem ser positivos.");
		}

		if (numThreads > arraySize) {
			die("NÚMERO_THREADS deve ser menor ou igual a TAMANHO_ARRAY.");
		}

		array = new int[arraySize];
		threads = new Thread[numThreads];

		System.out.println("Array inicializado com zeros.");

		for (int i = 0; i < numThreads; i++) {
			threads[i] = new IncrementThread(i);
			threads[i].start();
		}

		for (int i = 0; i < numThreads; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {

			}
		}

		for (int i = 0; i < arraySize; i++) {
			if (array[i] != 1) {
				correct = false;
				break;
			}
		}

		System.out.printf("Resultado %s.\n", (correct) ? "correto" : "incorreto");
	}
}
