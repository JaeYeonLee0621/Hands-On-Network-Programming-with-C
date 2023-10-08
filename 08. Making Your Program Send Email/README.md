# [Email Server]

## [SMTP (= Mail transmission)]

- the protocol responsible for **delivering emails between servers**
- It is a text-based protocol operating on TCP port 25


- Retrieving your mail from your email service provider is a different issue than delivering email between service provider
- Webmail provider allow mailbox access through a web browser (HTTP or HTTPS)

> if) Gmail provider then uses SMTP to deliver the email to the Yahoo

- Gmail Server = SMTP relay
+) SMTP server should only relay mail for trusted users

- Many mail provider has a set of mail servers used to accept incoming mail and a sepearte set of mail servers used to accept outgoin mail from the users

## [POP]

- Downloading emails to a local device, typically with the option to delete them from the server

## [IMAP]

- access, view and manage emails directly on the email server

+) Many webmail services still use IMAP or other email protocols on the server side to manage email messages and offer compatibility with external email clients

## [HTTP]

- Like gmail, outlook and yahoo use HTTP, HTTPS for email retrieval and interaction through a web browser

<br/>

# [SMTP Security]

- Securing SMTP is done by having SMTP connections start out as plaintext on port 25
- The SMTP client then issues a `STARTTLS` command to upgrade to a secure, encrypted conenction
- This secure connection works by merely running the SMTP commands through a TLS layer


- Many deskop email clients use TCP ports `465 (SMTP + SSL)`, `587 (SMTP + TLS, commonly use)` for SMTP mail submission
- `ISP (Internet Service Providers)` prefer these alternative ports for mail submission and it allows them to block port 25 altogether

<br/>

# [Finding an Email Server]

ex) bob@example.com

- Domain name can potentially use multiple mail server
- One mail server can provide service for many domain names
- The mail server must be identified by performing a DNS lookup for an `MX record` 

```bash
$ nslookup -type=mx gmail.com
$ mx mx gmail.com
```

<br/>

# [SMTP dialog]

S: 220 mail.example.com SMTP server ready 
// 220: the server is ready

C: HELO mail.example.com 
// HELO: client identify itself to the server

S: 250 Hello mail.example.com [190.0.2.67] 
// 250: The requested command was accepted and completed successfully

C: MAIL FROM:<alice@example.net> 
// MAIL: specify who is sending the mail

S: 250 OK

C: RCPT TO:<bob@example.com> 
// RCPT: specify a recipient

S: 250 Accepted

C: DATA 
// DATA: initiate the transfer of the actual email

S: 354 Enter message, ending with "." on a line by itself 
// 354: Start sending the message

C: Subject: Re: The Cake
C: Don't forget to bring the cake!

S: 250 OK

C: QUIT

S: 221 closing connection 
// 221: The connection is closing


+) `MIME(Multipurpose Internet Mail Extension)`

- It is not specific to email servers, but it is a widely used standard that extends the capabilities of email to handle various types of media, including text, images, audio, video, and binary files

<br/>

# [Spam-blocking pitfalls]

- Many residential ISPs don't allow outgoing connections on port 25
- If your residential case, you may consider renting a virtual private server to run this code


## [Sender Policy Framework (SPF)]

- listing which servers can send mail for a given domain
- If a sending server isn't on the SPF list, then receiving SMTP servers reject their mail

## [DomainKeys Identified Mail (DKIM)]

- authenticate email using digital signature
- treat non-DKIM mail as spam

**[How to publish DKIM]**

1. Log in to your email server and find the DKIM key generation section

2. Generate a pair of **DKIM key (private and public key)** published in your DNS records for verification

3. Publish the public key in DNS

- in the DNS TXT record's value, paste the public DKIM key

4. Access the your email server and enter the private DKIM key

5. Test DKIM

- Send a test email from your domain to an email account

+) When you use Gmail for sending emails through your Google account, Google automatically handles the DKIM signing of your outgoing emails

## [Domain-based Message Authentication, Reporting and Conformance (DMARC)]

- **A feature that helps check the authentication of email messages including DKIM mechanism or SPF**
- domain to publish whether SPF and/or DKIM is required of mail originating from them, among other things
- Protecting their domains from email spoofing, phishig etc
- Provide mechanisms for receiving feedback and reports on email authentication results

<br/>

- If you're sending email without these, your email will either be rejected outright or labeled as spam
