/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

package mil.tatrc.physiology.utilities;

import java.util.*;
import javax.mail.*;
import javax.mail.internet.*;

import com.sun.mail.smtp.SMTPTransport;


public class EmailUtil
{
  /** Email Recipients */
	protected List<String> recipients = new ArrayList<String>();
	/** Who the email is from */
	protected String from="EmailUtil";
	/** Where this mail is coming from */
	protected String smtp="localhost";
	protected int    smtpPort=25;
	/** Email subject */
	protected String subject;

	/** Add a recipient to the email */
	public void addRecipient(String recipient)
	{
		this.recipients.add(recipient);
	}
	
	/** Who is the email from */
	public void setSender(String sender)
	{
	  this.from=sender;
	}
	
	/** Email subject*/
	public void setSubject(String subject)
	{
		this.subject = subject;
	}
	
	/** Set the STMP host server */
	public void setSMTP(String smtp)
	{
	  this.smtp=smtp;
	  this.smtpPort=25;
	}
	
	/** Set the STMP host server */
  public void setSMTP(String smtp, int port)
  {
    this.smtp=smtp;
    this.smtpPort=port;
  }
	
	/** 
	 * Sends html to provided recipients
	 * and other configuration data
	 * @param content
	 */
   public void sendHTML(String content)
   {
     try
     {
       // Get system properties
       Properties properties = System.getProperties();
       // Setup mail server
       properties.setProperty("mail.smtp.host", this.smtp);
       properties.setProperty("mail.smtp.port", String.valueOf(this.smtpPort));
       // Get the default Session object.
       Session session = Session.getDefaultInstance(properties);
       //session.setDebug(true);
       Transport trans = session.getTransport("smtp");
       // Override the host for HELO or EHLO
       if (smtp != null && trans instanceof SMTPTransport)
       {
         ((SMTPTransport) trans).setLocalHost("SED-ABRAY.ara.wan");
       }
       // Create a default MimeMessage object.
       MimeMessage message = new MimeMessage(session);
       // Set From: header field of the header.
       message.setFrom(new InternetAddress(this.from));
       for(String recipient : this.recipients)
       {
         // Set To: header field of the header.
         message.addRecipient(Message.RecipientType.TO,new InternetAddress(recipient));
       }
       // Set Subject: header field
       message.setSubject(this.subject);
       // Send the actual HTML message, as big as you like
       message.setContent(content,"text/html");
       // Send message
       Transport.send(message);
     }
     catch (MessagingException mex) 
     {
       Log.error(mex);
     }
   }
   
   public static void main(String[] args)
   {
  	 EmailUtil emailer = new EmailUtil();
  	 emailer.setSender("abray@ara.com");
  	 emailer.addRecipient("abray@ara.com");
  	 emailer.addRecipient("boday@ara.com");
  	 emailer.setSMTP("smtp.ara.wan");
  	 emailer.setSubject("Test Email");
  	 emailer.sendHTML("<html><title>Test</title><body>Test Email</body></html>");
   }
}
