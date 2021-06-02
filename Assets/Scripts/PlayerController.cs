using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    public GameManager gameManager;
    [SerializeField] private Animator animator;
    [SerializeField] private Rigidbody rigid;

    public float moveSpeed = 3f;
    public LayerMask groundLayer;
    [SerializeField] bool isGrounded;

    [SerializeField] private Vector2 axis;

    public HashSet<string> npcList;



    private void Awake()
    {
        animator = GetComponent<Animator>();
        rigid = GetComponent<Rigidbody>();

        isGrounded = true;

        npcList = new HashSet<string>();
    }
    public void SetAxis(Vector2 axis)
    {
        this.axis = axis;
        animator.SetFloat("Speed", axis.magnitude);

        float horizontal = axis.x;
        float vertical = axis.y;

        animator.SetFloat("X", horizontal);
        animator.SetFloat("Y", vertical);

        StopCoroutine("Move");
        StartCoroutine("Move");

    }

    IEnumerator Move()
    {
        while (axis.magnitude > 0f)
        {
            float horizontal = axis.x;
            float vertical = axis.y;

            transform.Translate(Vector3.forward * vertical * Time.deltaTime * moveSpeed);
            transform.Rotate(Vector3.up * horizontal, Space.World);
            yield return new WaitForFixedUpdate();
        }

        yield return null;
    }

    public void TryJump()
    {

        if (isGrounded)
        {
            animator.SetTrigger("Jump");
            isGrounded = false;
            rigid.AddForce(Vector3.up * 300f);
            StartCoroutine(Jump());
        }

    }

    IEnumerator Jump()
    {
        yield return new WaitForSeconds(0.5f);

        while (!Physics.CheckSphere(transform.position, 0.2f, groundLayer))
        {
            yield return new WaitForFixedUpdate();
        }

        animator.SetTrigger("Land");
        isGrounded = true;
        yield return null;
    }

    public void FindNPC(string npcName)
    {
        npcList.Add(npcName);
    }

    public void RemoveNPC(string npcName)
    {
        npcList.Remove(npcName);

    }

}
