import java.util.concurrent.*;
public class SumTask extends RecursiveTask<Long>
{
	static final int THRESHOLD = 1000;
	private int begin;
	private int end;
	private int[] array;
	public SumTask(int begin, int end, int[] array) {
		this.begin = begin;
		this.end = end;
		this.array = array;
	}
	protected Long compute() {
		if (end - begin < THRESHOLD) {
			long sum = 0;
			for (int i = begin; i <= end; i++)
				sum += array[i];
			return sum;
		}
		else {
			try {
				int mid = (begin + end) / 2;
				SumTask leftTask = new SumTask(begin, mid, array);
				SumTask rightTask = new SumTask(mid + 1, end, array);
				leftTask.fork();
				rightTask.fork();
				return (long)rightTask.join() + (long)leftTask.join();
			}
			catch(Exception e){

			}

		}
		return 0L;
	}

	public static void main(String args[]) {
		ForkJoinPool pool = new ForkJoinPool();
		// array contains the integers to be summed
		int[] array = new int[1000000];
		for (int i = 0; i < array.length; i++) {
			array[i] = i * 2;
		}
		SumTask task = new SumTask(0, array.length - 1, array);
		long sum = pool.invoke(task);
		System.out.println(sum);
	}
}

