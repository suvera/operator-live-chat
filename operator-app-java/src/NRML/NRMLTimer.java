/**
 * @author nramanarayana
 *
 */
package NRML;

import javax.swing.Timer;
import java.awt.event.ActionListener;


public class NRMLTimer {
	
	private Timer timer;
	
	/**
	 * @param delay
	 * @param listener
	 */
	public NRMLTimer(int delay, ActionListener listener) {
		timer = new Timer(delay, listener);
	}
	
	/**
	 * Start
	 */
	public void start() {
		timer.start();
	}
	
	/**
	 * Stop
	 */
	public void stop() {
		timer.stop();
	}
}
