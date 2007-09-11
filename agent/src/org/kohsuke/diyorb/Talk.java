package org.kohsuke.diyorb;

import org.kohsuke.diyorb.impl.ProgramParser;
import org.kohsuke.diyorb.impl.ProgramLexer;
import org.kohsuke.diyorb.impl.Program;

import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;

import antlr.ANTLRException;

/**
 * @author Kohsuke Kawaguchi
 */
public class Talk {
    public static void main(String[] args) throws Exception {
        String deviceFileName = args[0];
        String jobName = args[1];
        String lastIconName = "";
        while(true) {
            try {
                URL url = new URL(jobName+"/lastBuild/buildStatus");
                HttpURLConnection con = (HttpURLConnection) url.openConnection();
                // get the real URL of the gif, like http://kohsuke.sfbay/hudson/nocacheImages/48x48/blue.gif
                con.getResponseCode();  // this would handle redirection
                String realUrl = con.getURL().toExternalForm();
                String iconName = realUrl.substring(realUrl.lastIndexOf('/')+1);
                iconName = iconName.substring(0,iconName.length()-3)+"txt"; // gif->txt
                if(!iconName.equals(lastIconName)) {
                    // status changed. send the new command file
                    System.out.println(iconName);
                    lastIconName = iconName;
                    InputStream in = Talk.class.getResourceAsStream(iconName);
                    if(in==null) {
                        in = Talk.class.getResourceAsStream("rainbow.txt");
                    }
                    ProgramParser pp = new ProgramParser(new ProgramLexer(in));
                    Program p = pp.program();
                    FileOutputStream fos = new FileOutputStream(deviceFileName);
                    fos.write("C,0,0,0,0\n".getBytes());
                    fos.write("O,0,0,0\n".getBytes());
                    p.writeTo(fos);
                    fos.close();
                    in.close();
                }
                con.getInputStream().close();
            } catch (IOException e) {
                e.printStackTrace();
            } catch (ANTLRException e) {
                e.printStackTrace();
            }
            Thread.sleep(10000);
        }
    }
}