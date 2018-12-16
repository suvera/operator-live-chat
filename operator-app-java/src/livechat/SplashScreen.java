package livechat;
/**
 * 
 */

/**
 * @author nramanarayana
 *
 */
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JWindow;

class SplashScreen extends JWindow
    implements LiveConstants
{

    public SplashScreen(int i)
    {
        JPanel jpanel = new JPanel(new BorderLayout());
        ImageIcon imageicon = new ImageIcon(LiveConstants.IMAGE_ICON);
        jpanel.add(new JLabel(imageicon, 0));
        getContentPane().add(jpanel);
        addMouseListener(new MouseAdapter() {

            public void mousePressed(MouseEvent mouseevent)
            {
                dispose();
            }

        });
        this.pack();
        this.centerScreen();
        this.show();
        this.setVisible(true);
        if(i != 0)
        {
            try
            {
                Thread.sleep(i);
                dispose();
            }
            catch(InterruptedException interruptedexception) { }
        }
    }

    private void centerScreen()
    {
        Dimension dimension = Toolkit.getDefaultToolkit().getScreenSize();
        int i = (int)((dimension.getWidth() - (double)getWidth()) / 2D);
        int j = (int)((dimension.getHeight() - (double)getHeight()) / 2D);
        setLocation(i, j);
    }
}

