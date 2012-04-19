/*
 * Main_Frame.java
 *
 * Created on 09 May 2006, 10:10
 */

package project6;

import java.io.*;
/**
 *
 * @author  Abrie
 */
public class Main_Frame extends javax.swing.JFrame {
    
    enc_dec engine = null;
    FileReader f_in = null;
    FileOutputStream f_out = null;
    /** Creates new form Main_Frame */
    public Main_Frame() {
        initComponents();
        engine = new enc_dec();
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    private void initComponents() {//GEN-BEGIN:initComponents
        fileName = new javax.swing.JTextField();
        b_open = new javax.swing.JButton();
        saveName = new javax.swing.JTextField();
        b_save = new javax.swing.JButton();
        skoon = new javax.swing.JTextArea();
        kripto = new javax.swing.JTextArea();
        b_enc = new javax.swing.JButton();
        b_dec = new javax.swing.JButton();
        jLabel1 = new javax.swing.JLabel();
        jLabel2 = new javax.swing.JLabel();

        getContentPane().setLayout(new org.netbeans.lib.awtextra.AbsoluteLayout());

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Block Ciphers");
        fileName.setText("Enter a filename");
        getContentPane().add(fileName, new org.netbeans.lib.awtextra.AbsoluteConstraints(270, 10, 100, 20));

        b_open.setText("Open");
        b_open.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                b_openActionPerformed(evt);
            }
        });

        getContentPane().add(b_open, new org.netbeans.lib.awtextra.AbsoluteConstraints(380, 10, 80, -1));

        saveName.setText("Enter a filename");
        getContentPane().add(saveName, new org.netbeans.lib.awtextra.AbsoluteConstraints(270, 50, 100, 20));

        b_save.setText("Save");
        b_save.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                b_saveActionPerformed(evt);
            }
        });

        getContentPane().add(b_save, new org.netbeans.lib.awtextra.AbsoluteConstraints(380, 50, 80, -1));

        getContentPane().add(skoon, new org.netbeans.lib.awtextra.AbsoluteConstraints(10, 120, 200, 270));

        getContentPane().add(kripto, new org.netbeans.lib.awtextra.AbsoluteConstraints(260, 120, 200, 270));

        b_enc.setText("Encrypt");
        b_enc.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                b_encActionPerformed(evt);
            }
        });

        getContentPane().add(b_enc, new org.netbeans.lib.awtextra.AbsoluteConstraints(30, 30, -1, -1));

        b_dec.setText("Decrypt");
        b_dec.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                b_decActionPerformed(evt);
            }
        });

        getContentPane().add(b_dec, new org.netbeans.lib.awtextra.AbsoluteConstraints(120, 30, -1, -1));

        jLabel1.setText("Plain Text");
        getContentPane().add(jLabel1, new org.netbeans.lib.awtextra.AbsoluteConstraints(80, 100, -1, -1));

        jLabel2.setText("Encrypted Text");
        getContentPane().add(jLabel2, new org.netbeans.lib.awtextra.AbsoluteConstraints(320, 100, -1, -1));

        java.awt.Dimension screenSize = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
        setBounds((screenSize.width-499)/2, (screenSize.height-450)/2, 499, 450);
    }//GEN-END:initComponents

    private void b_saveActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_b_saveActionPerformed
        // TODO add your handling code here:
         try{
            FileWriter f_out = new FileWriter(saveName.getText());
            f_out.write(kripto.getText(),0,kripto.getText().length());
            System.out.println(kripto.getText());
            f_out.close();
         }catch (Exception e){
            System.out.println("Error writing to file");
         }
         
    }//GEN-LAST:event_b_saveActionPerformed

    private void b_openActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_b_openActionPerformed
        // TODO add your handling code here:
         try{
            File f = new File(fileName.getText());
            f_in = new FileReader(fileName.getText());
            File f2 = new File(fileName.getText()+".enc");
            f_out = new FileOutputStream(f2);
         }catch (Exception e){
            System.out.println("File Doesn't Exist");
         }
         System.out.println("Read: "+f_in.getEncoding()+" Write: ");
    }//GEN-LAST:event_b_openActionPerformed

    private void b_decActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_b_decActionPerformed
        // TODO add your handling code here:
        int status;
        skoon.setText("");
        kripto.setText("");
        FileInputStream stream = null;
        try{
            File f = new File(fileName.getText());
            stream = new FileInputStream(f);
        
            while ((status = stream.read()) != -1){
                skoon.append(""+(char)status);
                kripto.append(""+(char)engine.decode(status));
            }
        }catch (Exception e){

        }   
    }//GEN-LAST:event_b_decActionPerformed

    private void b_encActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_b_encActionPerformed
        // TODO add your handling code here:
        int status;
        skoon.setText("");
        kripto.setText("");
        try{
            while ((status = f_in.read()) != -1){
                skoon.append(""+(char)status);
                status = engine.encode(status);System.out.println(status);
                kripto.append(""+(char)status);
                f_out.write(status);
            }
            f_in.close();
            f_out.close();
        }catch (Exception e){
        
        }   
    }//GEN-LAST:event_b_encActionPerformed
    
     
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton b_dec;
    private javax.swing.JButton b_enc;
    private javax.swing.JButton b_open;
    private javax.swing.JButton b_save;
    private javax.swing.JTextField fileName;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JTextArea kripto;
    private javax.swing.JTextField saveName;
    private javax.swing.JTextArea skoon;
    // End of variables declaration//GEN-END:variables
    
}